
/**
 * `socket.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "sock/socket.h"
#include "sock/tcp.h"

int
sock_accept (socket_t *self) {
  int sfd = 0;
  socklen_t len = sizeof(self->addr);
  sfd = accept(self->fd, (struct sockaddr *) self->addr, &len);
  if (sfd < 0) { return perror("accept()"), sfd; }
  self->sfd = sfd;
  return sfd;
}

ssize_t
sock_read (socket_t *self, char *buf, size_t len) {
  ssize_t size = 0;
  size = read(self->sfd, buf, len);
  return size;
}

int
sock_bind (socket_t *self) {
  size_t len = sizeof(struct sockaddr_in);
  int rc = bind(self->fd, (struct sockaddr *) self->addr, len);
  if (rc < 0) { return perror("bind()"), rc; }
  return rc;
}
