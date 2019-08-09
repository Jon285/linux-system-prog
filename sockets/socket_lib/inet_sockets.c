#define _BSD_SOURCE

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "inet_sockets.h"

int inetConnect(const char *host, const char *service, int type) {
  struct addrinfo hints;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_protocol = 0;
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = type;

  struct addrinfo *result;
  int socket_fd = getaddrinfo(host, service, &hints, &result);

  if (socket_fd != 0) {
    errno = ENOSYS;
    return -1;
  }

  int socket_s;
  struct addrinfo *rp;
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    socket_s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (socket_s == -1)
      continue;

    if (connect(socket_s, rp->ai_addr, rp->ai_addrlen) != -1)
      break;

    close(socket_s);
  }

  freeaddrinfo(result);

  return (rp == NULL) ? -1 : socket_s;
}

static int inetPassiveSocket(const char *service, int type, socklen_t *addrlen,
                             bool do_listen, int backlog) {
  struct addrinfo hints;

  memset(&hints, 0, sizeof(struct addrinfo));
}

int inetListen(const char *service, int backlog, socklen_t *addrlen) {}
int inetBind(const char *service, int type, socklen_t *addrlen) {}
char *inetAddressStr(const struct sockaddr *addr, socklen_t addrlen,
                     char *addrStr, int addrStrLen) {}
