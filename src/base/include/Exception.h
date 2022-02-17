#pragma once

#include <exception>
#include <string>

namespace zhy {
class Exception : public std::exception {
 public:
  explicit Exception(const std::string& what);
  ~Exception() noexcept override = default;

  const char* what() const noexcept override { return message_.c_str(); }

  std::string StackTrace() const { return stack_tarce_; }

 private:
  void FillStackTrace();

  std::string message_;
  std::string stack_tarce_;
};

}  // namespace zhy
