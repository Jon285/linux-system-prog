#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#define _BSD_SOURCE
#include <netdb.h>

#include "readline.h"

#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
#define PORT_NUM "50000"
#define INT_LEN 5

int main(int argc, char** argv)
{
	uint32_t seq_num;
	char requested_string[INT_LEN];
	char granted_string[INT_LEN];
	socklen_t addrlen;

	struct sockaddr_storage client_address;
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	int server_sock, client_sock, optval; 
	long requested_lenght;
	
	char address_string[ADDRSTRLEN];
	char hostname[NI_MAXHOST];
	char service[NI_MAXSERV];

	if(argc > 1 && strcmp(argv[1], "--help") == 0)
	{
		printf("%s: [init-seq-num]\n", argv[0]);
		return 0;
	}

	seq_num = 0;

	if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		fprintf(stderr, "%s: Error disabling SIGPIPE.\n", argv[0]);
		return EXIT_FAILURE;
	}

	memset(&hints, 0, sizeof(sizeof(struct addrinfo)));
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_next = NULL;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;

	int s = getaddrinfo(NULL, PORT_NUM, &hints, &result);
	if(s != 0)
	{
		fprintf(stderr, "%s: Error getting address info: %s.\n", argv[0], gai_strerror(s));
		return EXIT_FAILURE;
	}

	optval = 1;
	for(rp = result; rp != NULL; rp = rp->ai_next)
	{
		server_sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(server_sock == -1)
			continue;

		if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		{
			fprintf(stderr, "%s: Error configuring server socket options.\n", argv[0]);
			return EXIT_FAILURE;
		}

		if(bind(server_sock, rp->ai_addr, rp->ai_addrlen) == 0)
			break;

		close(server_sock);
	}

	if(rp == NULL)
	{
		fprintf(stderr, "%s: Couldn't bind server socket to the address.\n", argv[0]);
		return EXIT_FAILURE;
	}

	if(listen(server_sock, SOMAXCONN) == -1)
	{
		fprintf(stderr, "%s: Couldn't listen to clients.\n", argv[0]);
		return EXIT_FAILURE;
	}

	freeaddrinfo(result);

	for(;;)
	{
		addrlen = sizeof(struct sockaddr_storage);
		client_sock = accept(server_sock, (struct sockaddr*) &client_address, &addrlen);

		if(client_sock == -1)
		{
			fprintf(stderr, "%s: Error accepting client request.\n", argv[0]);
			continue;
		}

		if(getnameinfo((struct sockaddr*) &client_address, addrlen, hostname, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
			snprintf(address_string, ADDRSTRLEN, "(%s, %s)", hostname, service);
		else
			snprintf(address_string, ADDRSTRLEN, "(?UNKWOWN?)");

		printf("Connection received from: %s", address_string);
		
		if(readline(client_sock, requested_string, INT_LEN) <= 0)
		{
			close(client_sock);
			continue;
		}

		requested_lenght = strtol(requested_string, NULL, 10);
		if(requested_lenght <= 0)
		{
			close(client_sock);
			continue;
		}

		snprintf(granted_string, INT_LEN, "%d\n", seq_num);
		if(write(client_sock, &granted_string, strlen(granted_string)) != strlen(granted_string))
			fprintf(stderr, "Error on write.\n");
		seq_num += requested_lenght;

		if(close(client_sock) == -1)
			fprintf(stderr, "Error closing connection.\n");
	}
}
