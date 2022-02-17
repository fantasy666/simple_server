#include "ProcessInfo.h"

#include <dirent.h>
#include <pwd.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <cstdio>  // snprintf
#include <cstdlib>
#include <sstream>
#include <thread>

#include "FileUtil.h"

namespace zhy {
namespace detail {
__thread int t_numOpenedFiles = 0;
int FdDirFilter(const struct dirent* d) {
  if (::isdigit(d->d_name[0])) {
    ++t_numOpenedFiles;
  }
  return 0;
}
__thread std::vector<pid_t>* t_pids = nullptr;
int TaskDirFilter(const struct dirent* d) {
  if (::isdigit(d->d_name[0])) {
    t_pids->push_back(atoi(d->d_name));
  }
  return 0;
}
int ScanDir(const char* dirpath, int (*filter)(const struct dirent*)) {
  struct dirent** namelist = nullptr;
  int result = ::scandir(dirpath, &namelist, filter, alphasort);
  assert(namelist == nullptr);
  return result;
}

Timestamp g_start_time = Timestamp::Now();
// assume those won't change during the life time of a process.
int g_clock_ticks = static_cast<int>(::sysconf(_SC_CLK_TCK));
int g_page_size = static_cast<int>(::sysconf(_SC_PAGE_SIZE));
}  // namespace detail

pid_t ProcessInfo::Pid() { return ::getpid(); }

string ProcessInfo::PidString() {
  char buf[32];
  snprintf(buf, sizeof buf, "%d", Pid());
  return buf;
}

uid_t ProcessInfo::Uid() { return ::getuid(); }

string ProcessInfo::UserName() {
  struct passwd pwd;
  struct passwd* result = nullptr;
  char buf[8192];
  const char* name = "unknownuser";

  getpwuid_r(Uid(), &pwd, buf, sizeof(buf), &result);
  if (result) {
    name = pwd.pw_name;
  }
  return name;
}

uid_t ProcessInfo::Euid() { return ::geteuid(); }

Timestamp ProcessInfo::StartTime() { return detail::g_start_time; }

int ProcessInfo::ClockTicksPerSecond() { return detail::g_clock_ticks; }

int ProcessInfo::PageSize() { return detail::g_page_size; }

bool ProcessInfo::IsDebugBuild() {
#ifdef NDEBUG
  return false;
#else
  return true;
#endif
}

string ProcessInfo::HostName() {
  // HOST_NAME_MAX 64
  // _POSIX_HOST_NAME_MAX 255
  char buf[256];
  if (::gethostname(buf, sizeof(buf)) == 0) {
    buf[sizeof(buf) - 1] = '\0';
    return buf;
  } else {
    return "unknownhost";
  }
}

string ProcessInfo::ProcName() { return ProcName(ProcStat()).data(); }

std::string_view ProcessInfo::ProcName(const string& stat) {
  size_t lp = stat.find('(');
  size_t rp = stat.rfind(')');
  if (lp != string::npos && rp != string::npos && lp < rp) {
    return std::string_view(stat.data() + lp + 1,
                            static_cast<int>(rp - lp - 1));
  } else {
    return {};
  }
}

string ProcessInfo::ProcStatus() {
  string result;
  FileUtil::ReadFile("/proc/self/status", 65536, &result);
  return result;
}

string ProcessInfo::ProcStat() {
  string result;
  FileUtil::ReadFile("/proc/self/stat", 65536, &result);
  return result;
}

string ProcessInfo::ThreadStat() {
  char buf[64];
  std::stringstream sin;
  sin << std::this_thread::get_id();
  snprintf(buf, sizeof(buf), "/proc/self/task/%s/stat", sin.str().c_str());
  string result;
  FileUtil::ReadFile(buf, 65536, &result);
  return result;
}

string ProcessInfo::ExePath() {
  string result;
  char buf[1024];
  ssize_t n = ::readlink("/proc/self/exe", buf, sizeof(buf));
  if (n > 0) {
    result.assign(buf, n);
  }
  return result;
}

int ProcessInfo::OpenedFiles() {
  detail::t_numOpenedFiles = 0;
  detail::ScanDir("/proc/self/fd", detail::FdDirFilter);
  return detail::t_numOpenedFiles;
}

int ProcessInfo::MaxOpenFiles() {
  struct rlimit rl;
  if (::getrlimit(RLIMIT_NOFILE, &rl)) {
    return OpenedFiles();
  } else {
    return static_cast<int>(rl.rlim_cur);
  }
}

ProcessInfo::Cpu_Time ProcessInfo::CpuTime() {
  ProcessInfo::Cpu_Time t;
  struct tms tms;
  if (::times(&tms) >= 0) {
    const double hz = static_cast<double>(ClockTicksPerSecond());
    t.user_seconds = static_cast<double>(tms.tms_utime) / hz;
    t.system_seconds = static_cast<double>(tms.tms_stime) / hz;
  }
  return t;
}

int ProcessInfo::NumThreads() {
  int result = 0;
  string status = ProcStatus();
  size_t pos = status.find("Threads:");
  if (pos != string::npos) {
    result = ::atoi(status.c_str() + pos + 8);
  }
  return result;
}

std::vector<pid_t> ProcessInfo::Threads() {
  std::vector<pid_t> result;
  detail::t_pids = &result;
  detail::ScanDir("/proc/self/task", detail::TaskDirFilter);
  detail::t_pids = nullptr;
  std::sort(result.begin(), result.end());
  return result;
}
}  // namespace zhy
