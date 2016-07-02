#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/trim.hpp>


#include "engine.hpp"
#include "command.hpp"
#include "callback.hpp"
#include "failure.hpp"


using namespace cli;
namespace bpo = boost::program_options;

namespace {

  template<class E>
  std::unique_ptr<CallbackFunction> make_callback(E* engine,
                                                  typename CallbackMemberFunction<E>::function_t func)
  {
    return std::unique_ptr<CallbackFunction>(new CallbackMemberFunction<E>(engine, func));
  }

  Command read_command(std::istream& is, std::ostream& os)
  {
    static auto is_valid_command = [](const std::string& str){
      return str.size() > 0 && str[0] != '#';
    };
    std::string buffer;
    do {
      os << "> ";
    } while( std::getline(is, buffer) && !is_valid_command(buffer) );
    os << Engine::EOT << std::flush;
    boost::trim_if(buffer, boost::is_space());
    return Command(buffer);
  }

}


// Public interface
//--------------------------------------------------------
Engine::Engine()
  : _quit_flag(false),
    _options("Options for CTI Engine")
{
  // Register options
  options().add_options()
    ("disable-logging", "Disable logging")
    ("log-file", bpo::value<std::string>()->default_value(Logger::DEFAULT_LOG_FILENAME), "Set log file")
    ("log-dir", bpo::value<std::string>()->default_value(Logger::DEFAULT_LOG_DIR), "Set log dir")
    ("help", "Show help")
  ;

  // Register commands
  register_callback("echo",
                    make_callback(this, &Engine::echo_command),
                    "Echo test");
  register_callback("list_commands",
                    make_callback(this, &Engine::list_commands_command),
                    "List registered commands");
  register_callback("help",
                    make_callback(this, &Engine::help_command),
                    "Show help");
  register_callback("quit",
                    make_callback(this, &Engine::quit_command),
                    "Quit the application");
}


Engine::~Engine() noexcept
{
  close_log();
}


void Engine::initialize(int argc, const char* argv[])
{
  // Parse program options (throwable)
  {
    bpo::store( bpo::parse_command_line(argc,argv,_options), _parsed_options );
    bpo::notify( _parsed_options );
  }

  // Handle help option
  if( parsed_options().count("help") ) {
    std::cout << options() << std::endl;
    _quit_flag = true;
  }
}


int Engine::main_loop(std::istream& is, std::ostream& os)
{
  if(_quit_flag) return EXIT_SUCCESS;  // for help

  if(!open_log()) return EXIT_FAILURE;

  try {
    while( !_quit_flag ){
      Command command = read_command( is, os );
      handle_command( command, os );
    }
  } catch( const std::exception& e ) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}


// Protected interface
//--------------------------------------------------------
bool Engine::is_registered(const std::string& command) const noexcept
{
  auto ite = _callback_list.find(command);
  return ite != _callback_list.end();
}


void Engine::register_callback(std::string command, std::unique_ptr<CallbackFunction> cbf, std::string help) noexcept
{
  auto ite = _callback_list.find(command);
  if( ite != _callback_list.end() ) {
    _callback_list.erase(ite);
    _help_list.erase(_help_list.find(command));
  }
  _callback_list.emplace(command, std::move(cbf));
  _help_list.emplace(std::move(command), std::move(help));
}


bool Engine::remove_callback(const std::string& command){
  if( !is_registered(command) ) return false;
  auto i = _callback_list.erase(command) & _help_list.erase(command);
  return i > 0;
}


// Private functions
//--------------------------------------------------------
bool Engine::open_log()
{
  // Initialize logger
  if(parsed_options().count("disable-logging")) {
    return true;
  }

  return _logger.open(parsed_options()["log-file"].as<std::string>(),
                      parsed_options()["log-dir"].as<std::string>());

}


void Engine::close_log()
{
  _logger.close();
}


void Engine::handle_command(Command& command, std::ostream& os)
{
  bool status = true;
  std::string response;

  auto handler = _callback_list.find(command.name());
  if(handler != _callback_list.end()) {
    try {
      auto& func = handler->second;
      (*func)( command );
      response = command.response();
    } catch( const Failure& f ) {
      status = false;
      response = f.what();
    } catch( const std::exception& e ) {
      status = false;
      response = e.what();
      _quit_flag = true;
    }
  } else {
    status = false;
    response = (boost::format("unknown command: %s") % command.name()).str();
  }

  if(status) {
    logger().info() << "Accept command: " << command.raw_string();
  } else {
    logger().error() << response;
  }

  if( response.size() == 0 || response[response.size()-1] != '\n' ) {
    response += '\n';
  }

  os << ( status ? '=' : '?' )
     << ' ' << response
     << EOT << '\n';
  os.flush();
}


// Default commands
//--------------------------------------------------------
void Engine::echo_command(Command& command)
{
  check_num_arguments_at_least(command, 1);
  command.response_stream() << command.argument(0);
  for(auto i = 1; i < command.num_arguments(); ++i) {
    command.response_stream() << ' ' << command.argument(i);
  }
}


void Engine::list_commands_command(Command& command)
{
  check_num_arguments_equal(command, 0);
  command.response_stream() << '\n';
  for(auto ite = _callback_list.cbegin(); ite != _callback_list.cend(); ++ite) {
    command.response_stream() << ite->first << '\n';
  }
}


void Engine::help_command(Command& command)
{
  size_t size = 0;
  for(auto ite = _callback_list.cbegin(); ite != _callback_list.cend(); ++ite) {
    size = std::max( size, ite->first.size() );
  }
  size += 2;
  for(auto ite = _callback_list.cbegin(); ite != _callback_list.cend(); ++ite) {
    const auto& name = ite->first;
    const auto& help = _help_list.find(name)->second;
    command.response_stream() << '\n' << name;
    for(auto i = name.size(); i < size; ++i) command.response_stream() << ' ';
    command.response_stream() << " : " << help;
  }
}


void Engine::quit_command(Command& command)
{
  check_num_arguments_equal(command, 0);
  _quit_flag = true;
}
