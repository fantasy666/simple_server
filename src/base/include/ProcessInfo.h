#pragma once

#include <sys/types.h>

#include <string_view>
#include <vector>

#include "Timestamp.h"
#include "Types.h"

namespace zhy {
namespace ProcessInfo {
pid_t Pid();
string PidString();
uid_t Uid();
string UserName();
uid_t Euid();
Timestamp StartTime();
int ClockTicksPerSecond();
int PageSize();
bool IsDebugBuild();  // constexpr

string HostName();
string ProcName();
std::string_view ProcName(const string& stat);

string ProcStatus();
string ProcStat();
string ThreadStat();
string ExePath();
int OpenedFiles();
int MaxOpenFiles();

struct Cpu_Time {
  double user_seconds;
  double system_seconds;

  Cpu_Time() : user_seconds(0.0), system_seconds(0.0) {}

  double Total() const { return user_seconds + system_seconds; }
};

Cpu_Time CpuTime();

int NumThreads();
std::vector<pid_t> Threads();

}  // namespace ProcessInfo

}  // namespace zhy
