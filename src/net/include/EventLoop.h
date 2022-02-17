#pragma once

#include <functional>

#include "Macros.h"

namespace zhy {
class Channel;
class Epoll;

class EventLoop {
 public:
  EventLoop();
  ~EventLoop();
  DISALLOW_COPY_AND_MOVE(EventLoop);

  void Loop();
  void UpdateChannel(Channel* channel);

 private:
  Epoll* ep_{nullptr};
  bool quit_{true};
};
}  // namespace zhy
