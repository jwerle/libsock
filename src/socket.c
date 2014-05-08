
/**
 * `socket.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include "sock/socket.h"
#include "sock/tcp.h"

socket_t *
sock_new (int domain, int type) {
  socket_t *self = NULL;
  int fd = 0;

  // socket
  fd = sock_raw_new(domain, type, 0);
  if (fd < 0) { return perror("sock_new"), NULL; }

  // alloc
  self = (socket_t *) malloc(sizeof(socket_t));
  sock_init(self, fd);

  if (NULL == self) { return NULL; }
  return self;
}

int
sock_raw_new (int domain, int type, int protocol) {
  return socket(domain, type, protocol);
}

socket_t *
sock_init (socket_t *self, int fd) {
  struct sockaddr_in *addr = NULL;

  // alloc
  addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

  if (NULL == self) { return NULL; }
  if (NULL == addr) { return NULL; }

  // address
  addr->sin_family = AF_INET;
  addr->sin_addr.s_addr = INADDR_ANY;

  // assign
  self->addr = addr;
  self->fd = fd;
  return self;
}

int
sock_accept (socket_t *self) {
  socklen_t len = sizeof(struct sockaddr_in);
  int sfd = accept(self->fd, (struct sockaddr *) self->addr, &len);
  if (sfd < 0) { return perror("sock_accept"), sfd; }
  self->sfd = sfd;
  return sfd;
}

char *
sock_read (socket_t *self) {
  ssize_t size = 0;
  char *buf = (char *) malloc(sizeof(buf));
  if (NULL == buf) { return NULL; }
sread:
  size = recv(self->sfd, buf, SOCK_BUFSIZE, 0);
  buf[size] = '\0';
  if (size < 0) { return free(buf), NULL; }
  else if (0 == size) { goto sread; }
  return buf;
}

int
sock_bind (socket_t *self) {
  size_t len = sizeof(struct sockaddr_in);
  int rc = bind(self->fd, (struct sockaddr *) self->addr, len);
  if (rc < 0) { return perror("sock_bind"), rc; }
  return rc;
}

int
sock_close (socket_t *self) {
  int rc = shutdown(self->fd, SHUT_RDWR);
  if (rc < 0) { rc = shutdown(self->sfd, SHUT_RDWR); }
  if (rc < 0) { return perror("sock_close"), rc; }
  return rc;
}

void
sock_free (socket_t *self) {
  if (NULL != self->addr) { free(self->addr); }
  free(self);
}
