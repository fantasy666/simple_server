#include "ThreadPool.h"

#include <iostream>

#include "Exception.h"

namespace zhy {
ThreadPool::ThreadPool(unsigned int size) : running_(true) {
  threads_.reserve(size);
  for (unsigned int i = 0; i < size; ++i) {
    threads_.emplace_back(
        std::thread(std::bind(&ThreadPool::RunInThread, this)));
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    running_ = false;
  }
  not_empty_.notify_all();
  for (auto& thr : threads_) {
    if (thr.joinable()) {
      thr.join();
    }
  }
}

void ThreadPool::RunInThread() {
  try {
    while (running_) {
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(mutex_);
        while (tasks_.empty() && running_) {
          not_empty_.wait(lock);
        }

        if (tasks_.empty() && !running_) {
          return;
        }
        task = tasks_.front();
        tasks_.pop();
      }
      task();
    }
  } catch (const Exception& ex) {
    std::cerr << "excption caught in ThreadPool" << std::endl;
    std::cerr << "reasion: " << ex.what() << std::endl;
    std::cerr << "stack trace: " << ex.StackTrace() << std::endl;
    abort();
  } catch (const std::exception& ex) {
    std::cerr << "exception caught in ThreadPool" << std::endl;
    std::cerr << "reason: " << ex.what() << std::endl;
    abort();
  } catch (...) {
    std::cerr << "exception caught in ThreadPool" << std::endl;
    abort();
  }
}
}  // namespace zhy
