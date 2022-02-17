#include "LogStream.h"

#include <algorithm>
#include <cinttypes>

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

namespace zhy {

// TODO(itoa): better itoa.
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wtautological-compare"
#else
#pragma GCC diagnostic ignored "-Wtype-limits"
#endif

namespace detail {
const char DIGITS[] = "9876543210123456789";
const char* zero = DIGITS + 9;
static_assert(sizeof(DIGITS) == 20, "wrong number of DIGITS");

const char DIGITS_HEX[] = "0123456789ABCDEF";
static_assert(sizeof(DIGITS_HEX) == 17, "wrong number of DIGITS_HEX");

template <typename T>
size_t Convert(char buf[], T value) {
  T i = value;
  char* p = buf;

  do {
    int lsd = static_cast<int>(i % 10);
    i /= 10;
    *p++ = zero[lsd];
  } while (i != 0);

  if (value < 0) {
    *p++ = '-';
  }
  *p = '\0';
  std::reverse(buf, p);

  return p - buf;
}

size_t ConvertHex(char buf[], uintptr_t value) {
  uintptr_t i = value;
  char* p = buf;

  do {
    int lsd = static_cast<int>(i % 16);
    i /= 16;
    *p++ = DIGITS_HEX[lsd];
  } while (i != 0);
  *p = '\0';
  std::reverse(buf, p);
  return p - buf;
}
template class FixedBuffer<kSmallBuffer>;
template class FixedBuffer<kLargeBuffer>;
}  // namespace detail
using detail::Convert;
using detail::ConvertHex;
using detail::FixedBuffer;

template <int SIZE>
const char* FixedBuffer<SIZE>::DebugString() {
  *cur_ = '\0';
  return data_;
}

void LogStream::StaticCheck() {
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<double>::digits10,
                "kMaxNumericSize is large enough");
  static_assert(
      kMaxNumericSize - 10 > std::numeric_limits<long double>::digits10,
      "kMaxNumericSize is large enough");
  static_assert(kMaxNumericSize - 10 > std::numeric_limits<uint64_t>::digits10,
                "kMaxNumericSize is large enough");
}

LogStream::LogStream() = default;

template <typename T>
void LogStream::FormatInteger(T v) {
  if (buffer_.Avail() >= kMaxNumericSize) {
    size_t len = Convert(buffer_.Current(), v);
    buffer_.Add(len);
  }
}

LogStream& LogStream::operator<<(int16_t v) {
  *this << static_cast<int32_t>(v);
  return *this;
}

LogStream& LogStream::operator<<(uint16_t v) {
  *this << static_cast<uint32_t>(v);
  return *this;
}

LogStream& LogStream::operator<<(int32_t v) {
  FormatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(uint32_t v) {
  FormatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(int64_t v) {
  FormatInteger(v);
  return *this;
}
LogStream& LogStream::operator<<(uint64_t v) {
  FormatInteger(v);
  return *this;
}

LogStream& LogStream::operator<<(const void* p) {
  uintptr_t v = reinterpret_cast<uintptr_t>(p);
  if (buffer_.Avail() >= kMaxNumericSize) {
    char* buf = buffer_.Current();
    buf[0] = '0';
    buf[1] = 'x';
    size_t len = ConvertHex(buf + 2, v);
    buffer_.Add(len + 2);
  }
  return *this;
}

LogStream& LogStream::operator<<(float v) {
  *this << static_cast<double>(v);
  return *this;
}

LogStream& LogStream::operator<<(double v) {
  if (buffer_.Avail() >= kMaxNumericSize) {
    int len = snprintf(buffer_.Current(), kMaxNumericSize, "%.12g", v);
    buffer_.Add(len);
  }
  return *this;
}

LogStream& LogStream::operator<<(char v) {
  buffer_.Append(&v, 1);
  buffer_.Add(1);
  return *this;
}

LogStream& LogStream::operator<<(const char* str) {
  if (str) {
    buffer_.Append(str, strlen(str));
  } else {
    buffer_.Append("(null)", 6);
  }
  return *this;
}

LogStream& LogStream::operator<<(const unsigned char* str) {
  return operator<<(reinterpret_cast<const char*>(str));
}

LogStream& LogStream::operator<<(const string& v) {
  buffer_.Append(v.c_str(), v.size());
  return *this;
}

LogStream& LogStream::operator<<(const std::string_view& v) {
  buffer_.Append(v.data(), v.size());
  return *this;
}

LogStream& LogStream::operator<<(const Buf& v) {
  *this << v.ToStringView();
  return *this;
}

template <typename T>
Fmt::Fmt(const char* fmt, T val) {
  static_assert(std::is_arithmetic<T>::value, "Must be arithmetic type");

  length_ = snprintf(buf_, sizeof(buf_), fmt, val);
  assert(static_cast<size_t>(length_) < sizeof(buf_));
}

template Fmt::Fmt(const char* fmt, char);

template Fmt::Fmt(const char* fmt, int16_t);
template Fmt::Fmt(const char* fmt, uint16_t);
template Fmt::Fmt(const char* fmt, int32_t);
template Fmt::Fmt(const char* fmt, uint32_t);
template Fmt::Fmt(const char* fmt, int64_t);
template Fmt::Fmt(const char* fmt, uint64_t);

template Fmt::Fmt(const char* fmt, float);
template Fmt::Fmt(const char* fmt, double);
}  // namespace zhy
