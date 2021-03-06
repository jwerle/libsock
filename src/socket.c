/**
 * `socket.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>
#include "sock.h"

socket_t *
sock_new (int domain, int type) {
  socket_t *self = NULL;
  int fd = 0;

  fd = sock_raw_new(domain, type, 0);
  if (fd < 0) { return sock_perror("sock_new"), NULL; }

  self = (socket_t *) malloc(sizeof(socket_t));
  self->domain = domain;
  self->sock_type = type;
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
  addr = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));

  if (NULL == addr) { return NULL; }

  memset(addr, 0, sizeof(struct sockaddr_in));

  self->type = 0;
  self->host = NULL;
  self->addr = addr;
  self->fd = fd;

  return self;
}

int
sock_accept (socket_t *self) {
  socklen_t len = sizeof(struct sockaddr_in);
  int sfd = accept(self->fd, (struct sockaddr *) self->addr, &len);
  if (sfd < 0) { return sock_perror("sock_accept"), sfd; }
  self->sfd = sfd;
  return sfd;
}

char *
sock_recv (socket_t *self, int cont) {
  ssize_t size = 0;
  char *buf = (char *) malloc(sizeof(buf) * SOCK_BUFSIZE);
  int fd = self->sfd > 0 ? self->sfd : self->fd;
  if (NULL == buf) { return NULL; }
read:
  size = recv(fd, buf, SOCK_BUFSIZE, 0);
  buf[size] = '\0';
  if (size < 0) return sock_perror("sock_recv()"), free(buf), NULL;
  else if (0 == size && 0 != cont) goto read;
  return buf;
}

char *
sock_recvfrom (socket_t *self) {
  ssize_t size = 0;
  char *buf = (char *) malloc(sizeof(buf) * SOCK_BUFSIZE);
  if (NULL == buf) { return NULL; }
read:
  size = recvfrom(
      self->fd,
      buf,
      SOCK_BUFSIZE,
      0,
      (struct sockaddr *) self->addr,
      (socklen_t *) sizeof(self->addr));
  buf[size] = '\0';
  if (size < 0) { return sock_perror("sock_recvfrom()"), free(buf), NULL; }
  else if (0 == size) goto read;
  return buf;
}

int
sock_read (socket_t *self, char *buf, size_t size) {
  int rc = 0;
  int len = 0;
  memset(buf, 0, size);
  len = read(self->fd, buf, size);
  if (len < 0) { return sock_perror("sock_read"), -1; }
  return len;
}

int
sock_bind (socket_t *self) {
  size_t len = sizeof(struct sockaddr_in);
  int rc = bind(self->fd, (struct sockaddr *) self->addr, len);
  if (rc < 0) { return sock_perror("sock_bind"), rc; }
  return rc;
}

int
sock_connect (socket_t *self) {
  int rc = connect(self->fd,
      (struct sockaddr *) self->addr, sizeof(struct sockaddr_in));
  if (rc < 0) { return sock_perror("sock_connect"), rc; }
  return rc;
}

int
sock_send (socket_t *self, char *buf) {
  int rc = 0;
  rc = send(self->fd, buf, strlen(buf), 0);
  return rc;
}

int
sock_write (socket_t *self, char *buf) {
  int fd = 0;
  int rc = 0;
  fd = self->sfd > 0 ? self->sfd : self->fd;
  write(fd, buf, strlen(buf) + 1);
  return rc;
}

int
sock_close (socket_t *self) {
  int rc = 0;
  int fd = self->sfd;
  if (0 == fd) { return -1; }
  rc = shutdown(fd, SHUT_RDWR);
  if (rc < 0) { return sock_perror("sock_close"), rc; }
  rc = close(fd);
  if (rc < 0) { return sock_perror("sock_close"), rc; }
  self->sfd = 0;
  return rc;
}

int
sock_shutdown (socket_t *self) {
  int rc = 0;
  if (self->sfd > 0) { rc = sock_close(self); }
  if (rc < 0) { return rc; }
  rc = shutdown(self->fd, SHUT_RDWR);
  if (rc < 0) { return sock_perror("sock_shutdown"), rc; }
  return rc;
}

void
sock_free (socket_t *self) {
  if (NULL != self->addr) { free(self->addr); }
  free(self);
}
