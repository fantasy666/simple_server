#include "FileUtil.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <iostream>

#include "Types.h"

namespace zhy::FileUtil {

ReadSmallFile::ReadSmallFile(std::string_view filename)
    : fd_(::open(filename.data(), O_RDONLY | O_CLOEXEC)), err_(0) {
  buf_[0] = '\0';
  if (fd_ < 0) {
    err_ = errno;
  }
}

ReadSmallFile::~ReadSmallFile() {
  if (fd_ >= 0) {
    ::close(fd_);
  }
}

template <typename String>
int ReadSmallFile::ReadToString(int maxSize, String* content, int64_t* fileSize,
                                int64_t* modifyTime, int64_t* createTime) {
  static_assert(sizeof(off_t) == 8, "_FILE_OFFSET_BITS = 64");
  assert(content != nullptr);
  int err = err_;
  if (fileSize) {
    struct stat statbuf {};
    if (::fstat(fd_, &statbuf) == 0) {
      if (S_ISREG(statbuf.st_mode)) {
        *fileSize = statbuf.st_size;
        content->reserve(static_cast<int>(
            std::min(implicit_cast<int64_t>(maxSize), *fileSize)));
      } else if (S_ISDIR(statbuf.st_mode)) {
        err = EISDIR;
      }
      if (modifyTime) {
        *modifyTime = statbuf.st_mtime;
      }
      if (createTime) {
        *createTime = statbuf.st_ctime;
      }
    } else {
      err = errno;
    }
    while (content->size() < implicit_cast<size_t>(maxSize)) {
      size_t to_read = std::min(
          implicit_cast<size_t>(maxSize) - content->size(), sizeof(buf_));
      ssize_t n = ::read(fd_, buf_, to_read);
      if (n > 0) {
        content->append(buf_, n);
      } else {
        if (n < 0) {
          err = errno;
        }
        break;
      }
    }
  }
  return err;
}

int ReadSmallFile::ReadToBuffer(int* size) {
  int err = err_;
  if (fd_ >= 0) {
    ssize_t n = ::pread(fd_, buf_, sizeof(buf_) - 1, 0);
    if (n >= 0) {
      if (size) {
        *size = static_cast<int>(n);
      }
      buf_[n] = '\0';
    } else {
      err = errno;
    }
  }
  return err;
}
template int ReadSmallFile::ReadToString(int maxSize, std::string* content,
                                         int64_t* fileSize, int64_t* modifyTIme,
                                         int64_t* createTime);

template int zhy::FileUtil::ReadFile(std::string_view filename, int maxSize,
                                     std::string* content, int64_t* fileSize,
                                     int64_t* modifyTIme, int64_t* createTime);

AppendFile::AppendFile(std::string_view filename)
    : fp_(::fopen(filename.data(), "ae")), written_bytes_(0) {
  assert(fp_);
  ::setbuffer(fp_, buffer_, sizeof(buffer_));
}

AppendFile::~AppendFile() { ::fclose(fp_); }

void AppendFile::Append(const char* logline, size_t len) {
  size_t written = 0;
  while (written != len) {
    size_t remain = len - written;
    size_t n = Write(logline + written, remain);
    if (n != remain) {
      int err = ferror(fp_);
      if (err) {
        std::cerr << "AppendFile::Append() failed" << strerror(err)
                  << std::endl;
        break;
      }
    }
    written += n;
  }
  written_bytes_ += written;
}

void AppendFile::Flush() { ::fflush(fp_); }

size_t AppendFile::Write(const char* logline, size_t len) {
  return ::fwrite_unlocked(logline, 1, len, fp_);
}

}  // namespace zhy::FileUtil
