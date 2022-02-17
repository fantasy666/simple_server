#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "Macros.h"

namespace zhy {
class ThreadPool {
 public:
  explicit ThreadPool(unsigned int size = std::thread::hardware_concurrency());
  ~ThreadPool();
  DISALLOW_COPY_AND_MOVE(ThreadPool);

  template <class F, class... Args>
  auto Run(F&& f, Args&&... args)
      -> std::future<typename std::result_of<F(Args...)>::type>;

 private:
  void RunInThread();

  std::vector<std::thread> threads_;
  std::queue<std::function<void()>> tasks_;
  std::mutex mutex_;
  std::condition_variable not_empty_;
  bool running_{false};
};

template <class F, class... Args>
auto ThreadPool::Run(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(mutex_);

    if (!running_) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    tasks_.emplace([task]() { (*task)(); });
  }

  not_empty_.notify_one();
  return res;
}

}  // namespace zhy
