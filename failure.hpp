/*!
 * @file  failure.hpp
 * @brief Exception handler
 */
#ifndef CLI_BASIC_ENGINE_FAILURE_HPP
#define CLI_BASIC_ENGINE_FAILURE_HPP

#include <iostream>
#include <sstream>


namespace cli {

  /*!
   * @brief   Failure thrown by callback functions or engine instance to treat internal errors
   * @note    Construction of Failure object with operator<< has non-standard semantics as follows;
   * @code
   * // OK : Construct with a message
   * throw Failure("message");
   *
   * // OK : Build failure object with multiple objects
   * throw Failure() << "message" << ...;
   *
   * // NG : Cannot modify directly
   * Failure failure;
   * failure << "message" << ...;
   * throw failure;
   *
   * // OK : Can modify through the handle of response_stream
   * Failure failure;
   * failure.response_stream() << "message" << ...;
   * throw Failure;
   * @endcode
   * It derives from GtpCommand used in the fuego project.
   */
  class Failure {

    public:
    /*!
     * @typedef message_type
     * @brief   Type of message
     */
    using message_type = std::string;
    /*!
     * @typedef stream_type
     * @brief   Type of response stream
     */
    using stream_type  = std::ostringstream;

    //! Default ctor.
    Failure() = default;

    //! Construct with a message
    explicit Failure(const message_type& message);

    //! Custom copy ctor.
    Failure(const Failure&);

    //! Default move ctor.
    Failure(Failure&&) = default;

    //! Returns internal response_stream handler
    stream_type& response_stream() noexcept
    {
      return _stream;
    }

    //! Returns failure response
    message_type what() const
    {
      return _stream.str();
    }

    /*!
     * @brief   Insert operator for rvalue
     * @tparam  T : type of inserted objects
     * @note    It returns a new Failure instance.
     */
    template<class T>
    Failure operator<<( T&& value ) &&
    {
      Failure copied = std::move(*this);
      copied.response_stream() << value;
      return copied;
    }

    //! Calling insert operator for lvalue is disabled
    template<class T>
    Failure& operator<<( T&& ) & = delete;


    private:
    stream_type _stream;

  };

}

#endif  /* CLI_BASIC_ENGINE_FAILURE_HPP */
