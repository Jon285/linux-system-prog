#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

#define SOCKET_PATH "/tmp/unix_server"
#define BUFFER_SIZE 100

int main(void)
{
	struct sockaddr_un address;
	int socket_fd;
	ssize_t num_read;
	char buffer[BUFFER_SIZE];

	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(socket_fd == -1)
	{
		fprintf(stderr, "Error creating socket.\n");
		exit(EXIT_FAILURE);
	}

	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;
	
	//if we are on linux, the use abstract socket names
	#ifdef __linux__
		strncpy(&address.sun_path[1], SOCKET_PATH, sizeof(address.sun_path) - 2);
	#else
		strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);
	#endif
	
	if(connect(socket_fd, (struct sockaddr*) &address, sizeof(struct sockaddr_un)) == -1)
	{
		fprintf(stderr, "Error connecting to the server.\n");
		exit(EXIT_FAILURE);
	}

	while((num_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
		if(write(socket_fd, buffer, num_read) != num_read)
		{
			fprintf(stderr, "Partial/failed write.\n");
			exit(EXIT_FAILURE);
		}

	if(num_read == -1)
	{
		fprintf(stderr, "Error reading input.\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
