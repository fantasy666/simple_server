#pragma once
#include <sys/types.h>

#include <string_view>

#include "Macros.h"
namespace zhy {
namespace FileUtil {
class ReadSmallFile {
 public:
  explicit ReadSmallFile(std::string_view filename);
  ~ReadSmallFile();
  DISALLOW_COPY_AND_MOVE(ReadSmallFile);

  template <typename String>
  int ReadToString(int maxSize, String* content, int64_t* fileSize,
                   int64_t* modifyTIme, int64_t* createTime);
  int ReadToBuffer(int* size);
  const char* Buffer() const { return buf_; }

  static const int kBufferSize = 64 * 1024;

 private:
  int fd_;
  int err_;
  char buf_[kBufferSize];
};

template <typename String>
int ReadFile(std::string_view filename, int maxSize, String* content,
             int64_t* fileSize = nullptr, int64_t* modifyTIme = nullptr,
             int64_t* createTime = nullptr) {
  ReadSmallFile file(filename);
  return file.ReadToString(maxSize, content, fileSize, modifyTIme, createTime);
}

class AppendFile {
 public:
  explicit AppendFile(std::string_view filename);
  ~AppendFile();
  DISALLOW_COPY_AND_MOVE(AppendFile);

  void Append(const char* logline, size_t len);
  void Flush();
  off_t WrittenBytes() const { return written_bytes_; }

 private:
  size_t Write(const char* logline, size_t len);

  FILE* fp_;
  char buffer_[64 * 1024];
  off_t written_bytes_;
};
}  // namespace FileUtil
}  // namespace zhy
