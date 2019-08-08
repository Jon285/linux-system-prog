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
	int socket_fd, input_fd;
	ssize_t num_read;
	char buffer[BUFFER_SIZE];

	socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(socket_fd == -1)
	{
		fprintf(stderr, "Error opening socket.\n");
		exit(EXIT_FAILURE);
	}

	if(remove(SOCKET_PATH) == -1 && errno != ENOENT)
	{
		fprintf(stderr, "Couldn't remove previous socket.\n");
		exit(EXIT_FAILURE);
	}

	memset(&address, 0, sizeof(struct sockaddr_un));
	address.sun_family = AF_UNIX;
	
	//if we are on linux, then use abstract socket names
	#ifdef __linux__
		strncpy(&address.sun_path[1], SOCKET_PATH, sizeof(address.sun_path) - 2);
	#else
		strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path) - 1);
	#endif

	if(bind(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_un)) == -1)
	{
		fprintf(stderr, "Error binding to socket.\n");
		exit(EXIT_FAILURE);
	}

	if(listen(socket_fd, SOMAXCONN) == -1)
	{
		fprintf(stderr, "Error listening to connection.\n");
		exit(EXIT_FAILURE);
	}

	for(;;)
	{
		input_fd = accept(socket_fd, NULL, NULL);
		if(input_fd == -1)
		{
			fprintf(stderr, "Error accepting connection.\n");
			exit(EXIT_FAILURE);
		}

		while((num_read = read(input_fd, buffer, BUFFER_SIZE)) > 0)
			if(write(STDOUT_FILENO, buffer, num_read) != num_read)
			{
				fprintf(stderr, "\nPartial/failed write.\n");
				exit(EXIT_FAILURE);
			}

		if(num_read == -1)
		{
			fprintf(stderr, "Error reading socket data.\n");
			exit(EXIT_FAILURE);
		}

		if(close(input_fd) == -1)
		{
			fprintf(stderr, "Error closing input socket!\n");
		}
	}
}
