
/**
 * `client.c' - libsock
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


struct client_s;

typedef void (on_response_t) (struct client_s *, socket_t *, char *);

typedef struct client_s {
  socket_t *socket;
  char *host;
  int port;
} client_t;

void
usage () {
  fprintf(stderr, "%s\n", "usage: client [-h] <host> <port>");
}

client_t *
client_new (char *host, int port) {
  client_t *self = NULL;
  socket_t *sock = NULL;

  self = (client_t *) malloc(sizeof(client_t));
  if (NULL == self) { return NULL; }

  sock = sock_tcp_client_new(host, port);

  self->socket = sock;
  return self;
}

int
client_connect (client_t *self) {
  int rc = sock_connect(self->socket);
  if (rc < 0) { return -1; }
  return 0;
}

int
client_read (client_t *self, size_t size, on_response_t *cb) {
  int rc = 0;
  char *buf = (char *) malloc(sizeof(char) * size);
  if (NULL == buf) { return -1; }
  rc = sock_read(self->socket, buf, size);
  if (rc < 0) { return free(buf), -1; }
  cb(self, self->socket, buf);
  free(buf);
  return rc;
}

int
client_write (client_t *self, char *buf) {
  int rc;
  rc = sock_write(self->socket, buf);
  return 0;
}

int
client_close (client_t *self) {
  return sock_close(self->socket);
}

void
client_free (client_t *self) {
  sock_free(self->socket);
  free(self);
}

void
on_res (client_t *self, socket_t *sock, char *buf) {
  printf("%s\n", buf);
}

int
main (int argc, char **argv) {
  client_t *client = NULL;
  char *host = NULL;
  int port = 0;
  int rc = 0;

  if (1 == argc) { return usage(), 1; }

  if ('-' == argv[1][0]) {
    switch (argv[1][1]) {
      case 'h': return usage(), 0;
      default: return e("Unknown option `%s'", argv[1]), usage(), 1;
    }
  }

  host = argv[1];

  if (NULL == host) {
    return e("Missing `%s'", "host"), 1;
  }

  port = atoi(argv[2]);

  if (0 == port) {
    return e("Invalid port `%s'", argv[1]), 1;
  }

  printf("%s:%d\n", host, port);
  printf("-------------\n\n");

  client = client_new(host, port);

  if (NULL == client) {
    return e("%s\n", "Error initializing client"), 1;
  }

  rc = client_connect(client);
  if (rc < 0) { return 1; }

  rc = client_write(client, "hi");
  if (rc < 0) { return 1; }

  rc = client_read(client, 4096, on_res);
  if (rc < 0) { return 1; }

  client_free(client);

  return 0;
}
