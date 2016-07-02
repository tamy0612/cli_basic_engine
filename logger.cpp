#include "logger.hpp"

#include <ios>
#include <iostream>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


using namespace cli;

namespace {

  std::string level_label(LogLevel level)
  {
    switch(level) {
      case LogLevel::DEBUG:
        return "DEBUG";
      case LogLevel::INFO:
        return "INFO ";
      case LogLevel::WARNING:
        return "WARN ";
      case LogLevel::ERROR:
        return "ERROR";
      case LogLevel::FATAL:
        return "FATAL";
      default:
        return "*****";
    }
  }

  std::string default_log_dir()
  {
    return boost::filesystem::current_path().string() + "/log";
  }

  std::string default_log_filename()
  {
    auto now  = boost::posix_time::second_clock::local_time();
    return boost::posix_time::to_iso_extended_string(now) + ".log";
  }

}


const std::string Logger::DEFAULT_LOG_DIR      = default_log_dir();
const std::string Logger::DEFAULT_LOG_FILENAME = default_log_filename();


Logger::LogStream::LogStream(LogLevel level, Logger& logger)
  : _out(logger._output_stream.rdbuf())
{
  auto label = level_label(level);
  auto id    = label[0];
  auto date  = boost::posix_time::second_clock::local_time();
  _buffer << boost::format("%1% [%2%] %3% : ") % id
                                          % boost::posix_time::to_simple_string(date)
                                          % label;
}


Logger::LogStream::LogStream(LogStream&& stream) noexcept
  : _out(stream._out.rdbuf())
{
  _buffer.copyfmt(stream._buffer);
  _buffer << stream._buffer.str();
  stream._out.rdbuf(nullptr);
}


Logger::LogStream::~LogStream()
{
  _out << _buffer.str() << std::endl;
}


Logger::Logger()
  : _output_stream(nullptr)
{}


Logger::~Logger()
{
  close();
}


bool Logger::open(const std::string& filename, const std::string& log_dir)
{
  if(_file_buffer.is_open()) {
    return true;
  }
  auto log_file_path = boost::filesystem::path(log_dir + "/" + filename);
  try {
    auto dir = log_file_path.parent_path();
    if(!boost::filesystem::exists(dir)) {
      boost::filesystem::create_directories(dir);
    }
    _file_buffer.open(log_file_path.string(), std::ios_base::out | std::ios_base::app);
    _output_stream.rdbuf(&_file_buffer);
    _output_stream.clear();
  } catch(const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return false;
  }
  info() << "Initialized";
  return true;
}


void Logger::close()
{
  info() << "Closed";
  if(_file_buffer.is_open()) {
    _file_buffer.close();
    _output_stream.rdbuf(nullptr);
  }
}


auto Logger::debug() -> LogStream
{
  return LogStream(LogLevel::DEBUG, *this);
}


auto Logger::info() -> LogStream
{
  return LogStream(LogLevel::INFO, *this);
}


auto Logger::warning() -> LogStream
{
  return LogStream(LogLevel::WARNING, *this);
}


auto Logger::error() -> LogStream
{
  return LogStream(LogLevel::ERROR, *this);
}


auto Logger::fatal() -> LogStream
{
  return LogStream(LogLevel::FATAL, *this);
}
