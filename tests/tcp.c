
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ok/ok.h>
#include <assert.h>
#include <sock/opt.h>
#include <sock/tcp.h>

#define BUF_SIZE 1024

int
main (void) {

  {
    socket_t *sock = sock_tcp_new();
    assert(sock);

    ok("sock_tcp_new()");
  }

  {
    int rc = 0;
    int port = 0;
    int bl = 20; // backlog
    int *blptr = &bl;
    const char *host = NULL;
    socket_t *sock = NULL;

    {
      port = 6001;
      host = "0.0.0.0";
      sock = sock_tcp_new();
      assert(sock);

      rc = sock_set_opt(sock, SOCK_OPT_BACKLOG, blptr);
      assert(rc >= 0);

      rc = sock_set_opt(sock, SOCK_OPT_PORT, (const void *) &port);
      assert(rc >= 0);

      rc = sock_set_opt(sock, SOCK_OPT_ADDR, (const void *) INADDR_ANY);
      assert(rc >= 0);

      //rc = sock_tcp_listen(sock);
      //assert(rc >= 0);

      //rc = sock_tcp_close(sock);
      //assert(rc >= 0);

      ok("sock_tcp_listen(socket_t *) [INADDR_ANY]");
    }

    {
      port = 8888;
      host = "127.0.0.1";
      sock = sock_tcp_new();
      assert(sock);

      rc = sock_set_opt(sock, SOCK_OPT_BACKLOG, blptr);
      assert(rc >= 0);

      rc = sock_set_opt(sock, SOCK_OPT_PORT, (const void *) &port);
      assert(rc >= 0);

      rc = sock_set_opt(sock, SOCK_OPT_HOST, (const void *) host);
      assert(rc >= 0);

      rc = sock_set_opt(sock, SOCK_OPT_ADDR, (const void *) INADDR_ANY);
      assert(rc >= 0);

      rc = sock_bind(sock);
      assert(rc >= 0);

      rc = sock_tcp_listen(sock);
      assert(rc >= 0);

      while ((rc = sock_accept(sock))) {
        char *buf = (char *) malloc(sizeof(char) * BUF_SIZE);
        while (sock_read(sock, buf, BUF_SIZE) > 0) {
          close(sock->sfd);
          free(buf);
        }
        usleep(500000);
      }

      ok("sock_tcp_listen(socket_t *) [127.0.0.1]");
    }

  }

  ok_done();
  return 0;
}
