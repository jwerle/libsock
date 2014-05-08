
/**
 * `socket.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include "sock/opt.h"
#include "sock/socket.h"
#include "sock/tcp.h"

socket_t *
sock_tcp_new () {
  int fd = sock_raw_new(SOCK_IP, SOCK_STREAM, 0);
  socket_tcp_t *self = (socket_tcp_t *) malloc(sizeof(socket_tcp_t));
  if (NULL == self) { return NULL; }
  sock_init((socket_t *) self, fd);
  self->backlog = 0;
  return (socket_t *) self;
}

int
sock_tcp_listen (socket_t *sock) {
  socket_tcp_t *self = (socket_tcp_t *) sock;
  int rc = listen(sock->fd, self->backlog);
  if (rc < 0) { return perror("sock_tcp_listen"), rc; }
  return rc;
}

