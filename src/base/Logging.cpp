#include "Logging.h"

#include <cassert>
#include <cerrno>
#include <sstream>
#include <thread>

namespace zhy {
__thread char t_errnobuf[512];
__thread char t_time[64];
__thread time_t t_last_second;

const char* strerror_tl(int savedErrno) {
  return strerror_r(savedErrno, t_errnobuf, sizeof(t_errnobuf));
}

Logger::Level InitLogLevel() {
  if (::getenv("ZHY_LOG_TRACE")) {
    return Logger::TRACE;
  } else if (::getenv("ZHY_LOG_DEBUG")) {
    return Logger::DEBUG;
  } else {
    return Logger::INFO;
  }
}

Logger::Level g_log_level = InitLogLevel();

const char* log_level_name[Logger::NUM_LOG_LEVELS] = {
    "TRACE ", "DEBUG ", "INFO  ", "WARN  ", "ERROR ", "FATAL "};

class T {
 public:
  T(const char* str, unsigned len) : str_(str), len_(len) {
    assert(strlen(str_) == len_);
  }
  const char* str_;
  const unsigned len_;
};

inline LogStream& operator<<(LogStream& s, T v) {
  s.Append(v.str_, static_cast<int>(v.len_));
  return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v) {
  s.Append(v.data_, v.size_);
  return s;
}

void DefaultOutput(const char* msg, int len) {
  size_t n = fwrite(msg, 1, len, stdout);
  (void)n;
}

void DefaultFlush() { fflush(stdout); }

Logger::output_func_ g_output = DefaultOutput;
Logger::flush_func_ g_flush = DefaultFlush;

Logger::Impl::Impl(Level level, int old_errno, const SourceFile& file, int line)
    : time_(Timestamp::Now()), level_(level), line_(line), basename_(file) {
  // 1. FormatTime()
  // 2. 往数据流里面写入线程信息
  FormatTime();
  std::stringstream sin;
  sin << std::this_thread::get_id();
  stream_ << sin.str() << ' ';
  stream_ << T(log_level_name[level_], 6);
  if (old_errno != 0) {
    stream_ << strerror_tl(old_errno) << "(errno=" << old_errno << ") ";
  }
}

void Logger::Impl::FormatTime() {
  // 获取当前的格式化时间，写入流中
  int64_t microSecondsSinceEpoch = time_.MicroSecondsSinceEpoch();
  time_t seconds = static_cast<time_t>(microSecondsSinceEpoch /
                                       Timestamp::kMicroSecondsPerSecond);
  int microseconds = static_cast<int>(microSecondsSinceEpoch %
                                      Timestamp::kMicroSecondsPerSecond);
  if (seconds != t_last_second) {
    t_last_second = seconds;
    struct tm tm_time;
    ::gmtime_r(&seconds, &tm_time);
    int len =
        snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d",
                 tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                 tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    assert(len == 17);
    (void)len;
  }

  Fmt us(".%06dZ ", microseconds);
  assert(us.Length() == 9);
  stream_ << T(t_time, 17) << T(us.Data(), 9);
}

void Logger::Impl::Finish() {
  stream_ << " - " << basename_ << ":" << line_ << '\n';
}

Logger::Logger(SourceFile file, int line) : impl_(INFO, 0, file, line) {}

Logger::Logger(SourceFile file, int line, Level level, const char* func)
    : impl_(level, 0, file, line) {
  impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, Level level)
    : impl_(level, 0, file, line) {}

Logger::Logger(SourceFile file, int line, bool toAbort)
    : impl_(toAbort ? FATAL : ERROR, errno, file, line) {}

Logger::~Logger() {
  impl_.Finish();
  const LogStream::Buf& buf(Stream().Buffer());
  g_output(buf.Data(), buf.Length());
  if (impl_.level_ == FATAL) {
    g_flush();
    abort();
  }
}

void Logger::SetLogLevel(Logger::Level level) { g_log_level = level; }
void Logger::SetOutput(output_func_ out) { g_output = out; }
void Logger::SetFlush(flush_func_ flush) { g_flush = flush; }
}  // namespace zhy
