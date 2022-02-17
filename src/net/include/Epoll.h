#pragma once

#include <vector>

#include "Macros.h"

#ifdef OS_LINUX
#include <sys/epoll.h>
#endif

namespace zhy {
class Channel;

class Epoll {
 public:
  Epoll();
  ~Epoll();
  DISALLOW_COPY_AND_MOVE(Epoll);

  void UpdateChannel(Channel* ch);
  void DeleteChannel(Channel* ch);

  std::vector<Channel*> Poll(int timeout = -1);

 private:
  int epfd_{-1};
  std::vector<struct epoll_event> events_;
};
}  // namespace zhy
