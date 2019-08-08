#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	int flags;
	int perms;
	int file;
	ssize_t num_read;
	char buffer[BUFFER_SIZE];

	if(argc < 2)
		return -1;

	int arg = getopt(argc, argv, "a");
	perms = S_IRUSR | S_IWUSR;

	if(arg == 'a')
	{
		flags = O_WRONLY | O_CREAT | O_APPEND;
		file = open(argv[2], flags, perms);
	}
	else
	{
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		file = open(argv[1], flags, perms);
	}

	while(1)
	{
		num_read = read(0, buffer, BUFFER_SIZE);
		if(num_read > 0)
		{
			write(file, buffer, num_read);
			write(1, buffer, num_read);
		}
	}

	return 0;
}
