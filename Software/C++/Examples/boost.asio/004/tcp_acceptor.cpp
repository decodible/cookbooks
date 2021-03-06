#include "tcp_acceptor.h"
#include "tcp_socket.h"

#include <sys/socket.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

tcp_acceptor::tcp_acceptor(io_service &io_service) : io_servcie_(io_service) {
  sfd = create_socket("3000");
  set_socket_non_blocking(sfd);

  io_servcie_.register_event(sfd, EPOLLIN | EPOLLERR | EPOLLET, &handler);

  listen(sfd, SOMAXCONN);
}

void tcp_acceptor::async_accept(const Handler &callback) {
  handler = std::bind(&tcp_acceptor::on_accept_ready, this, callback);
}

void tcp_acceptor::on_accept_ready(const Handler &callback) {
  if (callback) {
    callback();
  }
}
