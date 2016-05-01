#include "failure.hpp"


cli::Failure::Failure(const message_type& message)
  : _stream( message )
{}


cli::Failure::Failure(const Failure& failure)
  : _stream()
{
  _stream << failure.what();
  _stream.copyfmt( failure._stream );
}
