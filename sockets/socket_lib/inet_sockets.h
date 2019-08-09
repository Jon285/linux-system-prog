#pragma once

#include <netdb.h>
#include <sys/socket.h>

int inetConnect(const char* host, const char* service, int type);
int inetListen(const char* service, int backlog, socklen_t* addrlen);
int inetBind(const char* service, int type, socklen_t* addrlen);
char* inetAddressStr(const struct sockaddr* addr, socklen_t addrlen, char* addrStr, int addrStrLen);

#define IS_ADDR_STR_LEN 4096
