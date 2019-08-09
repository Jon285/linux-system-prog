#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define _BSD_SOURCE
#include <netdb.h>

#include "readline.h"

#define PORT_NUM "50000"
#define INT_LEN 5

int main(int argc, char** argv)
{
    char* required_string;
    char received_string[INT_LEN];
    int client_sock;
    ssize_t num_read;

    struct addrinfo hints;
    struct addrinfo *result, *rp;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
	printf("%s server-host [sequence-len]\n", argv[0]);
	return 0;
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    if (getaddrinfo(argv[1], PORT_NUM, &hints, &result) != 0) {
	fprintf(stderr, "Couldn't get connection address.\n");
	return EXIT_FAILURE;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
	client_sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
	if (client_sock == -1)
	    continue;

	if (connect(client_sock, rp->ai_addr, rp->ai_addrlen) != -1)
	    break;

	close(client_sock);
    }

    if (rp == NULL) {
	fprintf(stderr, "Couldn't estabilish a connection.\n");
	return EXIT_FAILURE;
    }

    freeaddrinfo(result);

    required_string = (argc > 2) ? argv[2] : "1";
    if (write(client_sock, required_string, strlen(required_string)) != strlen(required_string)) {
	fprintf(stderr, "Couldn't send data to server.\n");
	return EXIT_FAILURE;
    }

    if (write(client_sock, "\n", 1) != 1) {
	fprintf(stderr, "Couldn't append newline.\n");
	return EXIT_FAILURE;
    }

    num_read = readline(client_sock, received_string, INT_LEN);
    if (num_read == -1) {
	fprintf(stderr, "Couldn't retrieve server answer.\n");
	return EXIT_FAILURE;
    }

    if (num_read == 0) {
	fprintf(stderr, "Unexpected EOF received from server.\n");
	return EXIT_FAILURE;
    }

    printf("Sequece number: %s", received_string);

    return EXIT_SUCCESS;
}
