/*!
 * @file  command.hpp
 * @brief Command handler
 */
#ifndef CLI_BASIC_ENGINE_COMMAND_HPP
#define CLI_BASIC_ENGINE_COMMAND_HPP

#include <sstream>
#include <vector>

#include <cstdint>
#include <cassert>


namespace cli {

  /*!
   * @brief  Parsed command & response holder
   * @code
   * // Usage
   * std::string command_string;
   * std::getline( std::cin, command_string );
   *
   * // Parse with construction
   * Command command( command_string );
   * assert( command.raw_string() == command_string );
   *
   * // Re-using
   * std::string another_command;
   * command.parse( another_command );
   * assert( command.raw_string() == another_command );
   *
   * // add response
   * command.response_stream() << "Response message";
   * assert( command.response(), "Response message" );
   * @endcode
   */
  class Command {

    public:
    /*!
     * @typedef  command_type
     * @brief    Type of command line
     */
    using command_type = std::string;
    /*!
     * @typedef  argument_type
     * @brief    Type of arguments
     */
    using argument_type = std::string;
    /*!
     * @typedef  container_type
     * @brief    Container for arguments
     */
    using container_type = std::vector<argument_type>;
    /*!
     * @typedef  response_type
     * @brief    Type of response
     */
    using response_type = std::string;
    /*!
     * @typedef  stream_type
     * @brief    Type of response stream
     */
    using stream_type = std::ostringstream;


    public:
    //! Default ctor.
    Command() = default;

    /*!
     * @brief      Ctor. with raw command string
     * @param[in]  command : the given command string through CLI
     */
    explicit Command(command_type command_line);
    //! Move ctor.
    Command(Command&& command) noexcept;

    //! Move operator
    Command& operator=(Command&& command) noexcept;

    // Both copy ctor. and operator are deleted
    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    /*!
     * @brief      Parse command from a raw string to name and arguments
     * @param[in]  command   : raw command string
     * @note       It also clears response stream.
     */
    void parse(command_type command_line)
    {
      _raw_command = std::move(command_line);  parse_raw_command();
    }

    //! Clear response stream
    void clear() noexcept
    {
      _response_stream.clear();  _response_stream.str("");
    }

    //! Returns raw command string
    const command_type& raw_string() const noexcept
    {
      return _raw_command;
    }

    //! Returns command name (alias of the first argument)
    const argument_type& name() const noexcept
    {
      return _name;
    }

    //! Returns the number of arguments
    size_t num_arguments() const noexcept
    {
      return _arguments.size();
    }

    /*!
     * @brief      Returns the index-th argument as an argument_type
     * @param[in]  index : index value
     */
    const argument_type& argument(size_t index) const noexcept
    {
      return assert(index < num_arguments()), _arguments[index];
    }

    //! Returns response
    response_type response() const
    {
      return _response_stream.str();
    }

    //! Returns response stream handler to register response
    stream_type& response_stream() noexcept
    {
      return _response_stream;
    }


    private:
    void parse_raw_command();

    private:
    command_type   _raw_command;
    argument_type  _name;
    container_type _arguments;
    stream_type    _response_stream;

  };


  /*!
   * @brief      Check the number of arguments is equal to 'num'
   * @param[in]  command : checked command object
   * @param[in]  num     : condition
   * @param[in]  detail  : additional description for the error
   * @exception  Failure : thrown if command.num_arguments() != num
   */
  void check_num_arguments_equal(const Command& command, size_t num, const char* detail = nullptr) noexcept(false);

  /*!
   * @brief      Check the number of arguments is more than or equal to 'num'
   * @param[in]  command : checked command object
   * @param[in]  num     : condition
   * @param[in]  detail  : additional description for the error
   * @exception  Failure : thrown if command.num_arguments() < num
   */
  void check_num_arguments_at_least(const Command& command, size_t num, const char* detail = nullptr) noexcept(false);

  /*!
   * @brief      Check the number of arguments is less than or equal to 'num'
   * @param[in]  command : checked command object
   * @param[in]  num     : condition
   * @param[in]  detail  : additional description for the error
   * @exception  Failure : thrown if command.num_arguments() > num
   */
  void check_num_arguments_at_most(const Command& command, size_t num, const char* detail = nullptr) noexcept(false);

}

#endif  /* CLI_BASIC_ENGINE_COMMAND_HPP */
