#include "AsyncLogging.h"

#include <cassert>
#include <chrono>
#include <cstdio>

#include "LogFile.h"
#include "Timestamp.h"

namespace zhy {
/**
 * 1. 初始化的时候先给cur_buffer和next_buffer分配内存
 * 2. Append优先往cur_buffer写，如果空间不够
 *    (1) 先将cur_buffer存到buffers_中
 *    (2) 把next_buffer的空间给cur_buffer或者给cur_buffer分配空间
 *    (3) 往cur_buffer中写内容
 * 3. 线程内函数执行的流程
 *    (1) 初始化输出的日志文件LogFile, 分配两个空闲的buffer空间
 *        和用于转换的bufferToWrite空间
 *    (2) 在线程的循环中执行:
 *        A. 阻塞在buffers为空的条件下，直到到了刷新时间;
 *        B. Lock{将cur_buffer放入buffers，new_buffer1的空间由cur_buffer管理,
 *           buffers_to_write和buffers_交换，如果next_buffer为空，将new_buffer2分配给它}
 *        C. 将bufferToWrite中的buffer都写入LogFile中，
 *           然后给new_buffer1和new_buffer2留两个空间
 *        D. 将内容Flush
 */
AsyncLogging::AsyncLogging(const std::string& basename, off_t rollSize,
                           int flushInterval)
    : flush_interval_(flushInterval),
      running_(false),
      basename_(basename),
      roll_size_(rollSize),
      current_buffer_(new Buffer),
      next_buffer_(new Buffer) {
  current_buffer_->Bzero();
  next_buffer_->Bzero();
  buffers_.reserve(16);
};

AsyncLogging::~AsyncLogging() {
  if (running_) {
    Stop();
  }
}

void AsyncLogging::Append(const char* logline, int len) {
  std::unique_lock<std::mutex> lock(mutex_);
  if (current_buffer_->Avail() > len) {
    current_buffer_->Append(logline, len);
  } else {
    buffers_.push_back(std::move(current_buffer_));
    if (next_buffer_) {
      current_buffer_ = std::move(next_buffer_);
    } else {
      current_buffer_.reset(new Buffer);
    }
    current_buffer_->Append(logline, len);
    not_empty_.notify_one();
  }
}

void AsyncLogging::Start() {
  running_ = true;
  thread_ = std::thread(std::bind(&AsyncLogging::ThreadFunc, this));
}

void AsyncLogging::Stop() {
  running_ = false;
  not_empty_.notify_one();
  thread_.join();
}

void AsyncLogging::ThreadFunc() {
  assert(running_ == true);
  LogFile output(basename_, roll_size_, false);
  BufferPtr new_buffer1(new Buffer);
  BufferPtr new_buffer2(new Buffer);
  new_buffer1->Bzero();
  new_buffer2->Bzero();
  BufferVector buffers_to_write;
  buffers_to_write.reserve(16);
  while (running_) {
    assert(new_buffer1 && new_buffer1->Length() == 0);
    assert(new_buffer2 && new_buffer2->Length() == 0);
    assert(buffers_to_write.empty());

    {
      std::unique_lock<std::mutex> lock(mutex_);
      if (buffers_.empty()) {
        not_empty_.wait_for(lock, std::chrono::seconds(flush_interval_));
      }
      buffers_.push_back(std::move(current_buffer_));
      current_buffer_ = std::move(new_buffer1);
      buffers_to_write.swap(buffers_);
      if (!next_buffer_) {
        next_buffer_ = std::move(new_buffer2);
      }
    }

    assert(!buffers_to_write.empty());

    if (buffers_to_write.size() > 25) {
      char buf[256];
      snprintf(buf, sizeof(buf),
               "Dropped log messages at %s, %zd larger buffers\n",
               Timestamp::Now().ToFormattedString().c_str(),
               buffers_to_write.size() - 2);
      fputs(buf, stderr);
      output.Append(buf, static_cast<int>(strlen(buf)));
      buffers_to_write.erase(buffers_to_write.begin() + 2,
                             buffers_to_write.end());
    }
    for (const auto& buffer : buffers_to_write) {
      output.Append(buffer->Data(), buffer->Length());
    }
    if (buffers_to_write.size() > 2) {
      buffers_to_write.resize(2);
    }
    if (!new_buffer1) {
      assert(!buffers_to_write.empty());
      new_buffer1 = std::move(buffers_to_write.back());
      buffers_to_write.pop_back();
      new_buffer1->Reset();
    }
    if (!new_buffer2) {
      assert(!buffers_to_write.empty());
      new_buffer2 = std::move(buffers_to_write.back());
      buffers_to_write.pop_back();
      new_buffer2->Reset();
    }
    buffers_to_write.clear();
    output.Flush();
  }
  output.Flush();
}
}  // namespace zhy
