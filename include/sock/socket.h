
/**
 * `socket.h' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef SOCK_SOCKET_H
#define SOCK_SOCKET_H 1

#include <sys/socket.h>
#include "common.h"

#define SOCK_BUFSIZE 0x14000

#define SOCK_SOCKET_FIELDS   \
  int type:2;                \
  int fd:16;                 \
  int sfd;                   \
  int domain;                \
  int sock_type;             \
  int port;                  \
  socklen_t len;             \
  struct sockaddr_in *addr;  \
  struct hostent *host;      \
  char *host_s;              \
  char *service;             \

typedef struct socket_s {
  SOCK_SOCKET_FIELDS
} socket_t;

/**
 * Allocates a new `socket_t *' type
 * with an internal call to `socket(int, int, int)'
 * defaulting to `0' as the protocol family.
 * To create a socket with a set protocol family
 * use `sock_raw_new (int, int, int)' or
 * `socket(int, int, int)' and pass its
 * return value to `sock_init(int)' to get a
 * newly allocated `socket_t *'.
 */

SOCK_EXTERN socket_t *
sock_new (int, int);

/**
 * Calls `socket(int, int, int)' internally
 * and returns file descriptor.
 */

SOCK_EXTERN int
sock_raw_new (int, int, int);

/**
 * Initializes a `socket_t *.
 */

SOCK_EXTERN socket_t *
sock_init (socket_t *, int);

/**
 * Accepts a connection on a socket.
 */

SOCK_EXTERN int
sock_accept (socket_t *);

/**
 * Receives message from a connected socket
 * and returns a `char *' pointer. If the
 * second argument is `0' then it returns
 * a buffer after a single call to `recv'
 */

SOCK_EXTERN char *
sock_recv (socket_t *, int);

/**
 * Receives a message from a socket.
 */

SOCK_EXTERN char *
sock_recvfrom (socket_t *);

/**
 * Reads a message from socket into
 * `char *' buffer of `size_t' size.
 */

SOCK_EXTERN int
sock_read (socket_t *, char *, size_t);

/**
 * Binds a socket.
 */

SOCK_EXTERN int
sock_bind (socket_t *);

/**
 * Connects to a socket.
 */

SOCK_EXTERN int
sock_connect (socket_t *);

/**
 * Sends a message to a socket.
 */

SOCK_EXTERN int
sock_send (socket_t *, char *);

/**
 * Writes to a socket.
 */

SOCK_EXTERN int
sock_write (socket_t *, char *);

/**
 * Closes an open socket
 * descriptor.
 */

SOCK_EXTERN int
sock_close (socket_t *);

/**
 * Shuts down a socket and
 * closes the open file descriptor
 * if none zero.
 */

SOCK_EXTERN int
sock_shutdown (socket_t *);

/**
 * Free a socket from memory.
 */

SOCK_EXTERN void
sock_free (socket_t *);

#endif
