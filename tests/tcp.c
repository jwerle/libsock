
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/ip.h>
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
    sock_free(sock);

    ok("sock_tcp_new()");
  }

  {
    int rc = 0;
    int bl = 20; // backlog

    {
      int port = 6001;
      char *host = "127.0.0.1";
      socket_t *sock = sock_tcp_new();
      assert(sock);

      rc = sock_set_opt(sock, SOCK_OPT_BACKLOG, (const void *) &bl);
      assert(rc >= 0);

      rc = sock_set_opt(sock, SOCK_OPT_PORT, (const void *) &port);
      assert(rc >= 0);

      rc = sock_set_opt(sock, SOCK_OPT_ADDR, (const void *) INADDR_ANY);
      assert(rc >= 0);

      rc = sock_set_opt(sock, SOCK_OPT_HOST, (const void *) host);
      assert(rc >= 0);

      rc = sock_bind(sock);
      assert(rc >= 0);

      rc = sock_tcp_listen(sock);
      assert(rc >= 0);

      sock_free(sock);

      ok("sock_tcp_listen(socket_t *) [INADDR_ANY]");
    }

    {
      int reads = 0;
      int limit = 10;
      int port = 8888;
      char *host = "127.0.0.1";
      char *buf = NULL;
      socket_t *sock = sock_tcp_new();
      assert(sock);

      rc = sock_set_opt(sock, SOCK_OPT_BACKLOG, (const void *) &bl);
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
        if (reads == limit) { break; }
        buf = sock_read(sock);
        if (NULL == buf) {
          sock_close(sock);
          free(buf);
          return perror("sock_read()"), 1;
        }
        free(buf);
        sock_close(sock);
        usleep(5000);
        printf("%c[2K\r  ...reads: (+%d/%d)", 27,++ reads, limit);
        fflush(stdout);
      }

      printf("\n");

      sock_free(sock);

      ok("sock_tcp_listen(socket_t *) [127.0.0.1]");
    }

  }

  ok_done();
  return 0;
}
