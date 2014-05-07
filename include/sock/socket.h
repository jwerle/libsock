
/**
 * `socket.h' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef SOCK_SOCKET_H
#define SOCK_SOCKET_H 1

#include <sys/socket.h>
#include "common.h"

#define SOCK_SOCKET_FIELDS   \
  int type;                  \
  int fd;                    \
  int sfd;                   \
  socklen_t len;             \
  struct sockaddr_in *addr;  \

typedef struct socket_s {
  SOCK_SOCKET_FIELDS
} socket_t;

/**
 * Accepts a connection on `socket_t *'
 */

SOCK_EXTERN int
sock_accept (socket_t *);

/**
 * Reads message of `size_t size` from `socket_t *' into a
 * buffer
 */

SOCK_EXTERN ssize_t
sock_read (socket_t *, char *, size_t);

/**
 * Binds socket
 */

SOCK_EXTERN int
sock_bind(socket_t *);

#endif
