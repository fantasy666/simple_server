#include "Exception.h"

#include <memory>

#include "cxxabi.h"
#include "execinfo.h"

namespace zhy {

Exception::Exception(const std::string& what) : message_(what) {
  FillStackTrace();
}

void Exception::FillStackTrace() {
  const int max_frames = 200;  // 返回堆栈框架的最大个数
  void* frame[max_frames];     // 存放堆栈框架的的返回地址
  int nptrs = ::backtrace(frame, max_frames);
  char** strings = ::backtrace_symbols(frame, nptrs);  // 字符串数组
  if (strings == nullptr) {
    return;
  }

  size_t len = 256;
  char* demangled =
      static_cast<char*>(::malloc(len));  // 用于存放demangled以后的结果
  for (int i = 1; i < nptrs; ++i) {
    char* left_par = nullptr;  // 左括号
    char* plus = nullptr;      // 加号
    for (char* p = strings[i]; *p != '\0';
         ++p) {  // 找到左括号和加号的位置，二者之间的内容是须要demangle的
      if (*p == '(') {
        left_par = p;
      } else if (*p == '+') {
        plus = p;
      }
    }
    *plus = '\0';
    int status = 0;
    char* ret = abi::__cxa_demangle(left_par + 1, demangled, &len, &status);
    *plus = '+';
    if (status != 0) {
      stack_tarce_.append(strings[i]);
    } else {
      demangled = ret;
      stack_tarce_.append(strings[i], left_par + 1);
      stack_tarce_.append(demangled);
      stack_tarce_.append(plus);
    }

    stack_tarce_.push_back('\n');
  }
  free(demangled);
  free(strings);
}

}  // namespace zhy
