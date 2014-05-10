
/**
 * `opt.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include "sock/socket.h"
#include "sock/tcp.h"
#include "sock/dgram.h"
#include "sock/opt.h"

int
sock_set_opt (socket_t *sock, int opt, const void *value) {

  // sock types
  socket_tcp_t *tcp = (socket_tcp_t *) sock;
  //socket_dgram_t *dgram = NULL;

  // opt values
  struct hostent *host = NULL;
  int port = 0;

  switch (opt) {
    case SOCK_OPT_BACKLOG:
      switch (sock->type) {
        case SOCK_TYPE_TCP: tcp->backlog = (int) value; break;
        default: return -1;
      }
      break;
    case SOCK_OPT_PORT:
      port = *((int *)value);
      tcp->addr->sin_port = htons(port);
      break;

    case SOCK_OPT_ADDR:
      tcp->addr->sin_addr.s_addr = (uint32_t) htonl((uint32_t) value);
      break;

    case SOCK_OPT_HOST:
      host = gethostbyname((char *)value);
      if (NULL == host) { return -1; }
      bcopy(
          (char *) host->h_addr,
          (char *) &tcp->addr->sin_addr,
           host->h_length);
      sock->host = host;
      break;

    default: return setsockopt(sock->fd,
                 ((struct sockaddr *) sock->addr)->sa_family,
                 opt, value, sizeof(value));
  }
  return 0;
}
