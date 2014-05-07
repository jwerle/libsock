
#include <stdlib.h>
#include <ok/ok.h>
#include <assert.h>
#include <sock/opt.h>
#include <sock/tcp.h>
#include <sock/stream.h>

int
main (void) {

  {
    int rc = 0;
    sock_stream_t *stream = sock_stream_new(SOCK_TYPE_TCP);
    assert(stream);
    assert(stream->socket);

    rc = sock_set_opt(stream->socket, SOCK_OPT_BACKLOG, (const void *) 20);
    assert(rc >= 0);

    rc = sock_tcp_listen(stream->socket);
    assert(rc >= 0);

    ok("sock_stream_new(int)");
  }

  ok_done();
  return 0;
}
