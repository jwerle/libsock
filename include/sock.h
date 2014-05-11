
/**
 * `soch.h' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef SOCK_H
#define SOCK_H 1

#ifdef SOCK_LIB

#include "sock/socket.h"
#include "sock/tcp.h"
#include "sock/opt.h"
#include "sock/dgram.h"

#else

#include "socket.h"
#include "tcp.h"
#include "opt.h"
#include "dgram.h"

#endif

#endif
