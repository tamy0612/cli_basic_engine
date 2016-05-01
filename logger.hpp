#ifndef CLI_BASIC_ENGINE_LOGGER_HPP
#define CLI_BASIC_ENGINE_LOGGER_HPP

#include <ostream>
#include <sstream>
#include <fstream>


namespace cli {

  enum struct LogLevel {
    DEBUG, INFO, WARNING, ERROR, FATAL, DISABLED
  };


  class Logger {

    public:
    static const std::string DEFAULT_LOG_DIR;
    static const std::string DEFAULT_LOG_FILENAME;

    class LogStream {

      public:
      LogStream(LogLevel level, Logger& logger);
      LogStream(const LogStream&) = delete;
      LogStream(LogStream&&) noexcept;
      ~LogStream();

      template<class T>
      LogStream& operator<<(const T& output)
      {
        _buffer << output;
        return *this;
      }

      private:
      std::ostringstream _buffer;
      std::ostream       _out;

    };

    explicit Logger(LogLevel level = LogLevel::INFO);
    ~Logger();

    bool open(const std::string& log_dir = DEFAULT_LOG_DIR,
              const std::string& filename = DEFAULT_LOG_FILENAME);
    void close();

    LogStream debug();
    LogStream info();
    LogStream warning();
    LogStream error();
    LogStream fatal();

    private:
    LogLevel     _level;
    std::ostream _output_stream;
    std::filebuf _file_buffer;

  };

}

#endif  /* CLI_BASIC_ENGINE_LOGGER_HPP */
