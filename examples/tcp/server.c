
/**
 * `server.c' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sock.h>

#define S(s) s

#define e(fmt, msg) ({                           \
  fprintf(stderr, "error: " S(fmt), msg);        \
  fprintf(stderr, "\n");                         \
})

char *message = NULL;
struct server_s;

typedef void (on_request_t) (struct server_s *, socket_t *, char *);

typedef struct server_s {
  socket_t *socket;
  on_request_t *cb;
  int active;
} server_t;


void
usage () {
  fprintf(stderr, "%s\n", "usage: server [-h] <port> [message]");
}

server_t *
server_new (int port, on_request_t *cb) {
  char *host = "0.0.0.0";
  server_t *self = (server_t *) malloc(sizeof(server_t));
  socket_t *sock = sock_tcp_new(host, port);
  int backlog = 20;
  int rc = 0;

  if (NULL == self) { return NULL; }
  if (NULL == sock) { return NULL; }

  rc = sock_set_opt(sock, SOCK_OPT_BACKLOG, (void *) &backlog);
  if (rc < 0) { return NULL; }
  rc = sock_set_opt(sock, SOCK_OPT_PORT, (void *) &port);
  if (rc < 0) { return NULL; }
  rc = sock_set_opt(sock, SOCK_OPT_HOST, host);
  if (rc < 0) { return NULL; }
  rc = sock_bind(sock);
  if (rc < 0) { return NULL; }

  self->active = 1;
  self->socket = sock;
  self->cb = cb;
  return self;
}

int
server_listen (server_t *self) {
  int rc = 0;
  char *buf = NULL;
  rc = sock_tcp_listen(self->socket);
  if (rc < 0) { return -1; }
  while (self->active && (rc = sock_accept(self->socket))) {
    buf = sock_recv(self->socket);
    if (NULL == buf) { return -1; }
    self->cb(self, self->socket, buf);
    free(buf);
    usleep(5000);
  }
  if (rc < 0) { return -1; }
  return rc;
}

void
server_free (server_t *self) {
  sock_free(self->socket);
  free(self);
}

int
server_shutdown (server_t *self) {
  int rc = 0;
  rc = sock_close(self->socket);
  if (rc < 0) { return rc; }
  self->active = 0;
  return rc;
}

void
on_req (server_t *self, socket_t *sock, char *buf) {

#define reply(str) ({                             \
  int rc = sock_write(sock, str);                 \
  if (rc < 0) { server_shutdown(self); return; }  \
})

  if (0 == strcmp("EXIT", buf)) {
    server_shutdown(self);
    return;
  }

  printf("< %s\n", buf);
  printf("> %s\n", message);

  reply("reply: ");
  reply(message);
  reply("\n");

  sock_close(sock);

#undef reply
}

int
main (int argc, char **argv) {
  server_t *server = NULL;
  char *msg = NULL;
  int port = 0;
  int rc = 0;

  if (1 == argc) { return usage(), 1; }

  if ('-' == argv[1][0]) {
    switch (argv[1][1]) {
      case 'h': return usage(), 0;
      default: return e("Unknown option `%s'", argv[1]), usage(), 1;
    }
  }

  port = atoi(argv[1]);

  if (0 == port) {
    return e("Invalid port `%s'", argv[1]), 1;
  }

  if (argc > 2 && NULL != argv[2]) {
    msg = argv[2];
  } else { msg = ""; }

  printf("0.0.0.0:%d\n", port);
  printf("------------\n");
  printf("REPLY: %s\n\n", msg);

  message = msg;
  server = server_new(port, &on_req);

  if (NULL == server) {
    return e("%s\n", "Error initializing server"), 1;
  }

  rc = server_listen(server);
  if (rc < 0) { return server_free(server), 1; }

  server_free(server);

  return 0;
}
