#include "LogFile.h"

#include "ProcessInfo.h"

namespace zhy {

LogFile::LogFile(const std::string& basename, off_t rollSize, bool threadSafe,
                 int flushInterval, int checkEveryN)
    : basename_(basename),
      roll_size_(rollSize),
      flush_interval_(flushInterval),
      check_every_n_(checkEveryN),
      count_(0),
      thread_safe_(threadSafe),
      start_of_period_(0),
      last_roll_(0),
      last_flush_(0) {
  assert(basename_.find('/') == string::npos);
  RollFile();
}

LogFile::~LogFile() = default;

void LogFile::Append(const char* logline, int len) {
  if (thread_safe_) {
    std::unique_lock<std::mutex> lock(mutex_);
    AppendUnlocked(logline, len);
  } else {
    AppendUnlocked(logline, len);
  }
}

void LogFile::Flush() {
  if (thread_safe_) {
    std::unique_lock<std::mutex> lock(mutex_);
    file_->Flush();
  }
}

void LogFile::AppendUnlocked(const char* logline, int len) {
  file_->Append(logline, len);

  if (file_->WrittenBytes() > roll_size_) {
    RollFile();
  } else {
    ++count_;
    if (count_ >= check_every_n_) {
      time_t now = ::time(nullptr);
      time_t this_peroid = now / kRollPerSeconds_ * kRollPerSeconds_;
      if (this_peroid != start_of_period_) {
        RollFile();
      } else if (now - last_flush_ > flush_interval_) {
        last_flush_ = now;
        file_->Flush();
      }
    }
  }
}

bool LogFile::RollFile() {
  time_t now = 0;
  string filename = GetLogFileName(basename_, &now);
  time_t start = now / kRollPerSeconds_ * kRollPerSeconds_;

  if (now > last_roll_) {
    last_roll_ = now;
    last_flush_ = now;
    start_of_period_ = start;
    file_.reset(new FileUtil::AppendFile(filename));
    return true;
  }
  return false;
}

std::string LogFile::GetLogFileName(const std::string& basename, time_t* now) {
  std::string filename;
  filename.reserve(basename.size() + 64);
  filename = basename;

  char timebuf[32];
  struct tm tm {};
  *now = ::time(nullptr);
  ::gmtime_r(now, &tm);
  strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
  filename += timebuf;

  filename += ProcessInfo::HostName();
  char pidbuf[32];
  snprintf(pidbuf, sizeof(pidbuf), ".%d", ProcessInfo::Pid());
  filename += pidbuf;

  filename += ".log";

  return filename;
}
}  // namespace zhy
