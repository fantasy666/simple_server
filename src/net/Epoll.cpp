#include "Epoll.h"

#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstring>

#include "Channel.h"
#include "util.h"

#define MAX_EVENTS 1000
using zhy::Channel;
using zhy::Epoll;

Epoll::Epoll() : events_(MAX_EVENTS) {
  epfd_ = epoll_create1(0);
  Errif(epfd_ == -1, "epoll create failed");
}

Epoll::~Epoll() {
  if (epfd_ != -1) {
    close(epfd_);
    epfd_ = -1;
  }
}

std::vector<Channel*> Epoll::Poll(int timeout) {
  std::vector<Channel*> active_channels;
  int nfds = ::epoll_wait(epfd_, &*events_.begin(),
                          static_cast<int>(events_.size()), timeout);
  Errif(nfds == -1, "epoll wait error");

  for (auto it_fd = events_.begin(); it_fd != events_.end() && nfds > 0;
       ++it_fd) {
    if (it_fd->events > 0) {
      --nfds;
      Channel* ch = (Channel*)it_fd->data.ptr;
      ch->SetReadyEvents(it_fd->events);
      active_channels.push_back(ch);
    }
  }
  return active_channels;
}

void Epoll::UpdateChannel(Channel* channel) {
  int fd = channel->GetFd();
  struct epoll_event ev {};
  ev.data.ptr = channel;
  ev.events = channel->GetListenEvents();
  if (!channel->GetInEpoll()) {
    Errif(epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add error");
    channel->SetInEpoll();
  } else {
    Errif(epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify error");
  }
}

void Epoll::DeleteChannel(Channel* ch) {
  int fd = ch->GetFd();
  Errif(epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr) == -1,
        "epoll delete error");
  ch->SetInEpoll(false);
}
