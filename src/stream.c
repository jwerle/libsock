
/**
 * `stream.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <sys/socket.h>
#include "sock/socket.h"
#include "sock/tcp.h"
#include "sock/stream.h"

sock_stream_t *
sock_stream_new (int type) {
  sock_stream_t *stream = (sock_stream_t *) malloc(sizeof(sock_stream_t));
  if (NULL == stream) { return NULL; }
  switch (type) {
    case SOCK_TYPE_TCP:
      stream->socket = sock_tcp_new(SOCK_IP, SOCK_STREAM);
      break;
    default:
      // @TODO - handle with error
      return NULL;
  }

  if (NULL == stream->socket) { return NULL; }
  return stream;
}
