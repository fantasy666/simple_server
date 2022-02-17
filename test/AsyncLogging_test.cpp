#include "AsyncLogging.h"

#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

#include "Logging.h"
#include "Timestamp.h"

off_t kRollSize = 500 * 1000 * 1000;

zhy::AsyncLogging* g_asyncLog = nullptr;

void asyncOutput(const char* msg, int len) { g_asyncLog->Append(msg, len); }

void bench(bool longLog) {
  zhy::Logger::SetOutput(asyncOutput);

  int cnt = 0;
  const int kBatch = 1000;
  zhy::string empty = " ";
  zhy::string longStr(3000, 'X');
  longStr += " ";

  for (int t = 0; t < 30; ++t) {
    zhy::Timestamp start = zhy::Timestamp::Now();
    for (int i = 0; i < kBatch; ++i) {
      LOG_INFO << "Hello 0123456789"
               << " abcdefghijklmnopqrstuvwxyz " << (longLog ? longStr : empty)
               << cnt;
      ++cnt;
    }
    zhy::Timestamp end = zhy::Timestamp::Now();
    printf("%f\n", zhy::TimeDifference(end, start) * 1000000 / kBatch);
    struct timespec ts = {0, 500 * 1000 * 1000};
    nanosleep(&ts, NULL);
  }
}

int main(int argc, char* argv[]) {
  //   {
  //     // set max virtual memory to 2GB.
  //     size_t kOneGB = 1000 * 1024 * 1024;
  //     rlimit rl = {2 * kOneGB, 2 * kOneGB};
  //     setrlimit(RLIMIT_AS, &rl);
  //   }

  printf("pid = %d\n", getpid());

  char name[256] = {'\0'};
  strncpy(name, argv[0], sizeof(name) - 1);
  zhy::AsyncLogging log(::basename(name), kRollSize);
  log.Start();
  g_asyncLog = &log;

  bool longLog = argc > 1;
  bench(longLog);
}
