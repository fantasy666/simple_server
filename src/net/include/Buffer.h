#pragma once

#include <string>

#include "Macros.h"

namespace zhy {
class Buffer {
 public:
  Buffer() = default;
  ~Buffer() = default;

  DISALLOW_COPY_AND_MOVE(Buffer);

  void Append(const char* str, int size);
  size_t Size();
  const char* ToStr();
  void Clear();
  void Getline();
  void SetBuf(const char* buf);

 private:
  std::string buf_;
};
}  // namespace zhy
