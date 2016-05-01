#ifndef CLI_BASIC_ENGINE_ENGINE_HPP
#define CLI_BASIC_ENGINE_ENGINE_HPP

#include <memory>
#include <iostream>
#include <boost/noncopyable.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include "hash_map.hpp"
#include "logger.hpp"


namespace cli {

  class Command;
  class CallbackFunction;

  /*!
   * @brief  Basic application engine of the common text interface
   * @code
   * > Input stream starts with '>', and terminates with <CR>.
   * > # comment should be started with '#', and it will be ignored.
   *
   * = Response stream starts with '=' or '?', and end with a control character EOT.
   * ? Starting with '?', it means failure response. '=' is used for normal (success) response.
   *   After starting with '=' or '?', there can be multi-line response until EOT is found.
   *
   * Any other strings, not starts with '>', not between '=' or '?' and EOT, are ignored.
   * @endcode
   */
  class Engine : private boost::noncopyable {

    /*!
     * @typedef  callback_list
     * @brief    Type of container for callback functions
     */
    using callback_list = HashMap<std::unique_ptr<CallbackFunction>>;
    /*!
     * @typedef  help_list
     * @brief    Type of container for help comments
     */
    using help_list     = HashMap<std::string>;


    public:
    /*!
     * @var    EOT
     * @brief  Control character to notify the end of transmission
     */
    static constexpr char EOT = '\004';


    /*!
     * @brief  Default ctor.
     * @note   Default commands are registered in the constructor.
     */
    Engine();

    /*!
     * @brief  virtual dtor.
     * @note   Custom ctor. should be implemented in derived engine classes.
     */
    virtual ~Engine() noexcept;

    /*!
     * @brief      Initialize the engine
     * @param[in]  argc : The number of the parameters
     * @param[in]  argv : List of parameters
     * @exception
     */
    void initialize(int argc, const char* argv[]) noexcept(false);

    /*!
     * @brief      Run main loop for the interactive command line application
     * @param[in]  is : input stream  [default = std::cin]
     * @param[in]  os : output stream [default = std::cout]
     */
    int main_loop(std::istream& is = std::cin, std::ostream& os = std::cout);


    protected:
    // Accessors
    //--------------------------------------------------------
    /*!
     * @brief      Check the command is registered
     * @param[in]  command : command name
     */
    bool is_registered(const std::string& command) const noexcept;

    /*!
     * @brief      Alias function to register callback member function to the engine
     * @param[in]  command : command name
     * @param[in]  cbf     : pointer to the callback member function
     * @param[in]  help    : [optional] help comment for the command
     * @attention  It overwrites the existent same name command.
     */
    void register_callback(std::string command,
                           std::unique_ptr<CallbackFunction> cbf,
                           std::string help = "No help") noexcept;

    /*!
     * @brief      Remove registered command
     * @param[in]  command : command name
     * @retval     true  : the command is successfully removed
     * @retval     false : it is failed to remove the command
     */
    bool remove_callback(const std::string& command);

    //! Accessor to container of program options
    auto options() noexcept -> boost::program_options::options_description&
    {
      return _options;
    }

    //! Accessor to container of parsed program options
    auto parsed_options() const noexcept -> const boost::program_options::variables_map&
    {
      return _parsed_options;
    }

    Logger& logger() noexcept
    {
      return _logger;
    }

    private:
    bool open_log();
    void close_log();

    //! Handle commands
    void handle_command(Command&, std::ostream&);


    // Default commands
    //--------------------------------------------------------
    /*!
     * @page commands Default commands
     * The available commands in the application are listed in this page.
     * Folllowing symbols are used to explain the commands.
     *  - (*) : required argument
     *  - [*] : optional argument
     * @section Default commands on CTI engine
     *  command        | description
     *  ---------------|--------------------------------------
     *  echo (message) | Echo test
     *  list_commands  | Show the list of registered commands
     *  help           | Show the help of each command
     *  quit           | Quit the application
     */
    void echo_command(Command&);
    void list_commands_command(Command&);
    void help_command(Command&);
    void quit_command(Command&);


    //--------------------------------------------------------
    // container for callbacks
    callback_list _callback_list;
    help_list     _help_list;
    // flags
    bool _quit_flag;
    // options
    boost::program_options::options_description _options;
    boost::program_options::variables_map       _parsed_options;
    // logger
    Logger _logger;

  };

}

#endif  /* CLI_BASIC_ENGINE_ENGINE_HPP */
