
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <ok/ok.h>
#include <assert.h>
#include <sock/opt.h>
#include <sock/tcp.h>

#define BUF_SIZE 1024

int
main (void) {

  int rc = 0;

  // server
  {
    int bl = 20; // backlog

    {
      socket_t *sock = sock_tcp_new();
      assert(sock);
      sock_free(sock);

      ok("sock_tcp_new()");
    }

    {
      int port = 6001;
      socket_t *sock = sock_tcp_new();
      assert(sock);

      rc = sock_set_opt(sock, SOCK_OPT_BACKLOG, (const void *) &bl);
      assert(rc >= 0);

      rc = sock_set_opt(sock, SOCK_OPT_PORT, (const void *) &port);
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
      int port = 6002;
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

      rc = sock_bind(sock);
      assert(rc >= 0);

      rc = sock_tcp_listen(sock);
      assert(rc >= 0);

      while ((rc = sock_accept(sock))) {
        buf = sock_recv(sock);
        if (NULL == buf) {
          rc = sock_close(sock);
          free(buf);
          return 1;
        }
        free(buf);
        rc = sock_close(sock);
        if (rc < 0) {
          sock_free(sock);
          return 1;
        }
        usleep(5000);
        printf("%c[3K\r  ...reads: (+%d/%d)", 27, ++reads, limit);
        fflush(stdout);
        if (reads == limit) { break; }
      }

      printf("\n");
      sock_free(sock);
      ok("sock_tcp_listen(socket_t *) [127.0.0.1]");
    }
  }

  // client
  {
    char *buf = (char *) malloc(sizeof(char) * BUF_SIZE);
    socket_t *client = sock_tcp_client_new("127.0.0.1", 6003);
    assert(client);

    strcat(buf, "request");

    rc = sock_connect(client);
    assert(rc >= 0);

    rc = sock_write(client, buf);
    assert(rc >= 0);

    rc = sock_read(client, buf, BUF_SIZE);
    assert(rc >= 0);

    assert(0 == strcmp("reply", buf));

    free(buf);

    sock_free(client);

    ok("sock_tcp_client_new(socket_t *)");
  }

  ok_done();
  return 0;
}
