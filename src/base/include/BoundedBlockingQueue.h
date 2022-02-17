#pragma once

#include <boost/circular_buffer.hpp>
#include <cassert>
#include <condition_variable>
#include <mutex>

#include "Macros.h"

namespace zhy {
template <typename T>
class BoundedBlockingQueue {
 public:
  explicit BoundedBlockingQueue(int maxSize)
      : not_empty_(), not_full_(), queue_(maxSize) {}
  ~BoundedBlockingQueue() = default;
  DISALLOW_COPY_AND_MOVE(BoundedBlockingQueue);

  void Put(const T& x) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.full()) {
      not_full_.wait(lock);
    }
    assert(!queue_.full());
    queue_.push_back(x);
    not_empty_.notify_one();
  }

  void Put(T&& x) {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.full()) {
      not_full_.wait(lock);
    }
    assert(!queue_.full());
    queue_.push_back(std::move(x));
    not_empty_.notify_one();
  }

  T Take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty()) {
      not_empty_.wait(lock);
    }
    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop_front();
    not_full_.notify_one();
    return front;
  }

  bool Empty() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  bool Full() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.full();
  }

  size_t Size() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size();
  }

  size_t Capacity() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.capacity();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable not_empty_;
  std::condition_variable not_full_;
  boost::circular_buffer<T> queue_;
};
}  // namespace zhy
