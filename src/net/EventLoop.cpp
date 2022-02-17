#include "EventLoop.h"

#include <vector>

#include "Channel.h"
#include "Epoll.h"

using zhy::EventLoop;

EventLoop::EventLoop() { ep_ = new Epoll(); }

EventLoop::~EventLoop() { delete ep_; }

void EventLoop::Loop() {
  while (!quit_) {
    std::vector<Channel*> chs = ep_->Poll();
    for (auto& ch : chs) {
      ch->HandleEvent();
    }
  }
}

void EventLoop::UpdateChannel(Channel* channel) { ep_->UpdateChannel(channel); }
