#pragma once

#include "LogStream.h"
#include "Timestamp.h"

namespace zhy {
class Logger {
 public:
  enum Level {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  class SourceFile {
   public:
    template <int N>
    SourceFile(const char (&arr)[N])  // NOLINT
        : data_(arr), size_(N - 1) {
      const char* slash = strchr(data_, '/');
      if (slash) {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }
    explicit SourceFile(const char* filename) : data_(filename) {
      const char* slash = strchr(data_, '/');
      if (slash) {
        data_ = slash + 1;
      }
      size_ = static_cast<int>(strlen(data_));
    }
    const char* data_;
    int size_;
  };

  Logger(SourceFile file, int line);
  Logger(SourceFile file, int line, Level level);
  Logger(SourceFile file, int line, Level level, const char* func);
  Logger(SourceFile file, int line, bool toAbort);
  ~Logger();

  LogStream& Stream() { return impl_.stream_; }

  static Level LogLevel();
  static void SetLogLevel(Level level);

  using output_func_ = void (*)(const char* msg, int len);
  using flush_func_ = void (*)();
  static void SetOutput(output_func_);
  static void SetFlush(flush_func_);

 private:
  class Impl {
   public:
    using Level = Logger::Level;
    Impl(Level level, int old_errno, const SourceFile& file, int line);
    void FormatTime();
    void Finish();

    Timestamp time_;
    LogStream stream_;
    Level level_;
    int line_;
    SourceFile basename_;
  };
  Impl impl_;
};

extern Logger::Level g_log_level;

inline Logger::Level Logger::LogLevel() { return g_log_level; }

#define LOG_TRACE                                    \
  if (zhy::Logger::LogLevel() <= zhy::Logger::TRACE) \
  zhy::Logger(__FILE__, __LINE__, zhy::Logger::TRACE, __func__).Stream()
#define LOG_DEBUG                                    \
  if (zhy::Logger::LogLevel() <= zhy::Logger::DEBUG) \
  zhy::Logger(__FILE__, __LINE__, zhy::Logger::DEBUG, __func__).Stream()
#define LOG_INFO                                    \
  if (zhy::Logger::LogLevel() <= zhy::Logger::INFO) \
  zhy::Logger(__FILE__, __LINE__).Stream()
#define LOG_WARN zhy::Logger(__FILE__, __LINE__, zhy::Logger::WARN).Stream()
#define LOG_ERROR zhy::Logger(__FILE__, __LINE__, zhy::Logger::ERROR).Stream()
#define LOG_FATAL zhy::Logger(__FILE__, __LINE__, zhy::Logger::FATAL).Stream()
#define LOG_SYSERR zhy::Logger(__FILE__, __LINE__, false).Stream()
#define LOG_SYSFATAL zhy::Logger(__FILE__, __LINE__, true).Stream()

const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  ::zhy::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char* names, T* ptr) {
  if (ptr == NULL) {
    Logger(file, line, Logger::FATAL).Stream() << names;
  }
  return ptr;
}

}  // namespace zhy
