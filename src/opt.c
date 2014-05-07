
/**
 * `opt.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include "sock/socket.h"
#include "sock/tcp.h"
#include "sock/dgram.h"
#include "sock/opt.h"

int
sock_set_opt (socket_t *sock, int opt, const void *value) {
  // sock types
  socket_tcp_t *tcp = NULL;
  socket_dgram_t *dgram = NULL;

  // opt values
  int port = 0;
  int addr = 0;

  switch (sock->type) {

    // tcp opts
    case SOCK_TYPE_TCP:
      tcp = (socket_tcp_t *) sock;
      switch (opt) {
        case SOCK_OPT_BACKLOG:
          tcp->backlog = (int) value;
          break;

        case SOCK_OPT_PORT:
          port = *((int *)value);
          tcp->addr->sin_port = htons(port);
          break;

        case SOCK_OPT_ADDR:
          tcp->addr->sin_addr.s_addr = (uint32_t) htonl((uint32_t) value);
          break;

        case SOCK_OPT_HOST:
          tcp->host = gethostbyname(value);
          tcp->addr->sin_addr = *(struct in_addr *) tcp->host->h_addr;
          break;

        default:
          setsockopt(
              sock->fd,
              ((struct sockaddr *) sock->addr)->sa_family,
              opt, value,
              sizeof(value));
      }
      break;

    default: return -1;
  }
  return 0;
}
