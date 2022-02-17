#pragma once
#include <arpa/inet.h>

#include <string_view>

#include "Macros.h"

namespace zhy {
class InetAddress {
 public:
  InetAddress();
  InetAddress(std::string_view ip, uint16_t port);
  explicit InetAddress(const struct sockaddr_in& addr);
  ~InetAddress() = default;

  void SetAddr(sockaddr_in addr);
  sockaddr_in GetAddr();
  const char* GetIp();
  uint16_t GetPort();

 private:
  struct sockaddr_in addr_ {};
};

class Socket {
 public:
  Socket();
  explicit Socket(int fd);
  ~Socket();
  DISALLOW_COPY_AND_MOVE(Socket);

  void Bind(InetAddress* addr);
  void Listen();
  int Accept(InetAddress* addr);

  void Connect(InetAddress* addr);

  void SetNonBlocking();
  int GetFd();

 private:
  int sockfd_{-1};
};
}  // namespace zhy
