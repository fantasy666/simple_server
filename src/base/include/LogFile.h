#pragma once

#include <memory>
#include <mutex>
#include <string>

#include "FileUtil.h"
#include "Macros.h"
#include "Types.h"

namespace zhy {

class LogFile {
 public:
  LogFile(const std::string& basename, off_t rollSize, bool threadSafe = true,
          int flushInterval = 3, int checkEveryN = 1024);
  ~LogFile();
  DISALLOW_COPY_AND_MOVE(LogFile);

  void Append(const char* logline, int len);
  void Flush();
  bool RollFile();

 private:
  void AppendUnlocked(const char* logline, int len);
  static std::string GetLogFileName(const std::string& basename, time_t* now);

  const std::string basename_;
  const off_t roll_size_;
  const int flush_interval_;
  const int check_every_n_;
  int count_;
  bool thread_safe_;

  std::mutex mutex_;
  time_t start_of_period_;
  time_t last_roll_;
  time_t last_flush_;
  std::unique_ptr<FileUtil::AppendFile> file_;

  static const int kRollPerSeconds_ = 60 * 60 * 24;
};

}  // namespace zhy
