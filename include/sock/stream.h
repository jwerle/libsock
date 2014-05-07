
/**
 * `stream.h' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef SOCK_STREAM_H
#define SOCK_STREAM_H 1

#include "common.h"
#include "socket.h"

#define SOCK_STREAM_FIELDS \
  socket_t *socket;

typedef struct sock_stream_s {
  SOCK_STREAM_FIELDS
} sock_stream_t;

SOCK_EXTERN sock_stream_t *
sock_stream_new (int);

#endif
