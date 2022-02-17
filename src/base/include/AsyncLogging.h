#pragma once

#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

#include "BlockingQueue.h"
#include "BoundedBlockingQueue.h"
#include "LogStream.h"

namespace zhy {
class AsyncLogging {
 public:
  AsyncLogging(const std::string& basename, off_t rollSize,
               int flushInterval = 3);
  ~AsyncLogging();
  void Append(const char* logline, int len);
  void Start();
  void Stop();

 private:
  void ThreadFunc();
  using Buffer = zhy::detail::FixedBuffer<zhy::detail::kLargeBuffer>;
  using BufferVector = std::vector<std::unique_ptr<Buffer>>;
  using BufferPtr = BufferVector::value_type;

  const int flush_interval_;
  std::atomic_bool running_;
  const string basename_;
  const off_t roll_size_;
  std::thread thread_;
  std::mutex mutex_;
  std::condition_variable not_empty_;
  BufferPtr current_buffer_;
  BufferPtr next_buffer_;
  BufferVector buffers_;
};

}  // namespace zhy
