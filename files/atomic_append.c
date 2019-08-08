#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int file, flags;
	char data;
	mode_t mode;

	if(argc < 3)
		return -1;

	flags = O_WRONLY | O_CREAT | O_APPEND;
	mode = S_IRUSR | S_IWUSR;

	file = open(argv[1], flags, mode);

	for(long bytes = atol(argv[2]); bytes > 0; bytes--)
	{
		//lseek(file, 0, SEEK_END);
		write(file, &data, 1);
	}

	close(file);

	return 0;
}
