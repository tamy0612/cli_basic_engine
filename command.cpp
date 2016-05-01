#include <boost/algorithm/string.hpp>

#include "command.hpp"
#include "failure.hpp"


using namespace cli;


Command::Command(command_type command)
  : _raw_command( std::move(command) )
{
  parse_raw_command();
}


Command::Command(Command&& command) noexcept
  : _raw_command( std::move(command._raw_command) ),
    _name( std::move(command._name) ),
    _arguments( std::move_if_noexcept(command._arguments) ),
    _response_stream( std::move_if_noexcept(command._response_stream) )
{
}


void Command::parse_raw_command()
{
  // Parse command to name and arguments
  typename Command::container_type buffer;
  boost::split( buffer, _raw_command, boost::is_space(), boost::algorithm::token_compress_on );
  _name = std::move( buffer[0] );
  _arguments.clear();
  for( auto i = 1; i < buffer.size(); ++i )
  {
    _arguments.emplace_back( std::move(buffer[i]) );
  }
  // Clear response stream
  _response_stream.str("");
  _response_stream.clear();
}


//--------------------------------------------------------
void cli::check_num_arguments_equal( const Command& command,
                                const size_t num,
                                const char* detail ) noexcept(false)
{
  if(command.num_arguments() == num) return;

  Failure failure;
  failure.response_stream() << "Command '" << command.name() << "' requires ";
  switch(num) {
    case 0:
      failure.response_stream() << "no argument";
      break;
    case 1:
      failure.response_stream() << "1 argument";
      break;
    default:
      failure.response_stream() << num << " arguments";
  }

  if(detail != nullptr) {
    failure.response_stream() << ": " << detail;
  }

  throw failure;
}


void cli::check_num_arguments_at_least( const Command& command,
                                        const size_t num,
                                        const char* detail ) noexcept(false)
{
  if(command.num_arguments() >= num) return;

  Failure failure;
  failure.response_stream() << "Command '" << command.name() << "' requires ";
  switch( num ) {
    case 0:
      failure.response_stream() << "no argument";
      break;
    case 1:
      failure.response_stream() << "1 argument at least";
      break;
    default:
      failure.response_stream() << num << " arguments at least";
  }

  if(detail != nullptr) {
    failure.response_stream() << ": " << detail;
  }

  throw failure;
}


void cli::check_num_arguments_at_most( const Command& command,
                                       const size_t num,
                                       const char* detail ) noexcept(false)
{
  if(command.num_arguments() <= num ) return;

  Failure failure;
  failure.response_stream() << "Command '" << command.name() << "' requires ";
  switch( num )
  {
    case 0:
      failure.response_stream() << "no argument";
      break;
    case 1:
      failure.response_stream() << "1 argument at most";
      break;
    default:
      failure.response_stream() << num << " arguments at most";
  }

  if(detail != nullptr){
    failure.response_stream() << ": " << detail;
  }

  throw failure;
}
