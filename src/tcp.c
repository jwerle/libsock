
/**
 * `socket.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include "sock/opt.h"
#include "sock/socket.h"
#include "sock/tcp.h"

socket_t *
sock_tcp_new () {
  socket_tcp_t *self = NULL;
  struct sockaddr_in *addr;

  // alloc
  self = (socket_tcp_t *) malloc(sizeof(socket_tcp_t));
  addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

  if (NULL == self) { return NULL; }
  if (NULL == addr) { return NULL; }

  // create
  self->fd = socket(SOCK_IP, SOCK_STREAM, 0);

  if (self->fd < 0) { return perror("socket()"), NULL; }

  // address
  memset(addr, 0, sizeof(*addr));
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = INADDR_ANY;

  // assign
  self->backlog = 0;
  self->addr = addr;
  self->host = NULL;

  return (socket_t *) self;
}

int
sock_tcp_listen (socket_t *sock) {
  socket_tcp_t *self = (socket_tcp_t *) sock;
  int rc = listen(sock->fd, self->backlog);
  if (rc < 0) { return perror("listen()"), rc; }
  return rc;
}

int
sock_tcp_close (socket_t *sock) {
  socket_tcp_t *self = (socket_tcp_t *) sock;
  int rc = shutdown(sock->fd, SHUT_RDWR);
  if (rc < 0) { return perror("shutdown()"), rc; }
  return rc;
}
