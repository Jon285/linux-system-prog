#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	int input_file;
	int output_file;
	int iflags;
	int oflags;
	int use_flags;
	int num_read;
	char buffer[BUFFER_SIZE];

	if(argc != 3)
		return -1;

	iflags = O_RDONLY;
	oflags = O_WRONLY | O_CREAT | O_TRUNC;
	use_flags = S_IRUSR | S_IWUSR;

	input_file = open(argv[1], iflags);
	output_file = open(argv[2], oflags, use_flags);

	while((num_read = read(input_file, buffer, BUFFER_SIZE)) > 0)
		if(write(output_file, buffer, num_read) == -1)
			return -1;

	close(input_file);
	close(output_file);
	
	return 0;
}
