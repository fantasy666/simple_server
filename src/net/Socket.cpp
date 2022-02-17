#include "Socket.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>

#include "util.h"

namespace zhy {
Socket::Socket() {
  sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
  Errif(sockfd_ == -1, "socket create error");
}

Socket::Socket(int fd) : sockfd_(fd) {
  Errif(sockfd_ == -1, "socket create error");
}

Socket::~Socket() {
  if (sockfd_ != -1) {
    close(sockfd_);
    sockfd_ = -1;
  }
}

void Socket::Bind(InetAddress* addr) {
  struct sockaddr_in addr_tmp = addr->GetAddr();
  Errif(::bind(sockfd_, (struct sockaddr*)&addr_tmp, sizeof(addr_tmp)) == -1,
        "socket bind error");
}

void Socket::Listen() {
  Errif(::listen(sockfd_, SOMAXCONN) == -1, "socket listen error");
}

int Socket::Accept(InetAddress* addr) {
  struct sockaddr_in addr_tmp {};
  socklen_t addr_len = sizeof(addr_tmp);
  int clnt_sockfd = ::accept(sockfd_, (struct sockaddr*)&addr_tmp, &addr_len);
  Errif(clnt_sockfd == -1, "socket accept error");
  addr->SetAddr(addr_tmp);
  return clnt_sockfd;
}

void Socket::Connect(InetAddress* addr) {
  struct sockaddr_in addr_tmp = addr->GetAddr();
  Errif(::connect(sockfd_, (sockaddr*)&addr_tmp, sizeof(addr_tmp)) == -1,
        "socket connect error");
}

void Socket::SetNonBlocking() {
  fcntl(sockfd_, F_SETFL, fcntl(sockfd_, F_GETFL) | O_NONBLOCK);
}

InetAddress::InetAddress() = default;
InetAddress::InetAddress(const struct sockaddr_in& addr) : addr_(addr) {}
InetAddress::InetAddress(std::string_view ip, uint16_t port) {
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = inet_addr(ip.data());
  addr_.sin_port = htons(port);
}

void InetAddress::SetAddr(sockaddr_in addr) { addr_ = addr; }

sockaddr_in InetAddress::GetAddr() { return addr_; }

const char* InetAddress::GetIp() { return inet_ntoa(addr_.sin_addr); }

uint16_t InetAddress::GetPort() { return ntohs(addr_.sin_port); }
}  // namespace zhy
