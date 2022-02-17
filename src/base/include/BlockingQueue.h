#pragma once

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "Macros.h"

namespace zhy {
template <typename T>
class BlockingQueue {
 public:
  using queue_type = std::queue<T>;

  BlockingQueue() : not_empty_(), queue_() {}
  ~BlockingQueue() = default;
  DISALLOW_COPY_AND_MOVE(BlockingQueue);

  void Put(const T& x) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(x);
    not_empty_.notify_one();
  }

  void Put(T&& x) {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push(std::move(x));
    not_empty_.notify_one();
  }

  T Take() {
    std::unique_lock<std::mutex> lock(mutex_);
    while (queue_.empty()) {
      not_empty_.wait(lock);
    }
    assert(!queue_.empty());
    T front(std::move(queue_.front()));
    queue_.pop();
    return front;
  }

  queue_type Drain() {
    queue_type tmp_queue;
    {
      std::unique_lock<std::mutex> lock(mutex_);
      tmp_queue = std::move(queue_);
      assert(queue_.empty());
    }
    return tmp_queue;
  }

  size_t Size() const {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size();
  }

 private:
  mutable std::mutex mutex_;
  std::condition_variable not_empty_;
  queue_type queue_;
};

}  // namespace zhy
