#pragma once

#include <cassert>
#include <cstring>
#include <string>
#include <string_view>

#include "Macros.h"
#include "Types.h"

namespace zhy {
namespace detail {
const int kSmallBuffer = 4000;
const int kLargeBuffer = 4000 * 1000;

template <int SIZE>
class FixedBuffer {
 public:
  FixedBuffer() : cur_(data_) {}
  ~FixedBuffer() = default;
  DISALLOW_COPY_AND_MOVE(FixedBuffer);

  void Append(const char* buf, size_t len) {
    if (implicit_cast<size_t>(Avail()) > len) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  const char* Data() const { return data_; }
  int Length() const { return static_cast<int>(cur_ - data_); }

  char* Current() { return cur_; }
  int Avail() const { return static_cast<int>(end() - cur_); }
  void Add(size_t len) { cur_ += len; }

  void Reset() { cur_ = data_; }
  void Bzero() { memZero(data_, sizeof(data_)); }

  const char* DebugString();
  std::string ToString() const { return std::string(data_, Length()); }
  std::string_view ToStringView() const {
    return std::string_view(data_, Length());
  }

 private:
  const char* end() const { return data_ + sizeof(data_); }

  char data_[SIZE];
  char* cur_;
};
}  // namespace detail

class LogStream {
 public:
  using self = LogStream;
  using Buf = detail::FixedBuffer<detail::kSmallBuffer>;
  LogStream();
  DISALLOW_COPY_AND_MOVE(LogStream);

  self& operator<<(bool v) {
    buffer_.Append(v ? "1" : "0", 1);
    return *this;
  }
  self& operator<<(int16_t);
  self& operator<<(uint16_t);
  self& operator<<(int32_t);
  self& operator<<(uint32_t);
  self& operator<<(int64_t);
  self& operator<<(uint64_t);
  self& operator<<(const void*);
  self& operator<<(float v);
  self& operator<<(double);
  self& operator<<(char v);
  self& operator<<(const char* str);
  self& operator<<(const unsigned char* str);
  self& operator<<(const string& v);
  self& operator<<(const std::string_view& v);
  self& operator<<(const Buf& v);

  void Append(const char* data, int len) { buffer_.Append(data, len); }
  const Buf& Buffer() const { return buffer_; }
  void ResetBuffer() { buffer_.Reset(); }

 private:
  void StaticCheck();

  template <typename T>
  void FormatInteger(T);

  Buf buffer_;

  static const int kMaxNumericSize = 48;
};

class Fmt {
 public:
  DISALLOW_COPY_AND_MOVE(Fmt);
  template <typename T>
  Fmt(const char* fmt, T val);
  const char* Data() const { return buf_; }
  int Length() const { return length_; }

 private:
  char buf_[32];
  int length_;
};

inline LogStream& operator<<(LogStream& s, const Fmt& fmt) {
  s.Append(fmt.Data(), fmt.Length());
  return s;
}
}  // namespace zhy
