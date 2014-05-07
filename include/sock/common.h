/**
 * `common.h' - libsock
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 */

#ifndef SOCK_COMMON_H
#define SOCK_COMMON_H 1

#include <netinet/ip.h>

#if __GNUC__ >= 4
# define SOCK_EXTERN __attribute__((visibility("default")))
#else
# define SOCK_EXTERN
#endif

// alias
#ifdef _WIN64

#elif _WIN32

#elif __APPLE__

#define SOCK_FS PF_UNIX
#define SOCK_IP PF_INET

#elif __linux

#define SOCK_FS AF_UNIX
#define SOCK_IP AF_INET

#elif __unix

#elif __posix
#else
#error "Unsupported platform"
#endif

// socket types type
#define X(t) SOCK_TYPE_ ## t
enum {
  X(TCP),
  X(DRGAM)
};
#undef X

#endif
