#include <unistd.h>
#include <errno.h>

#include "readline.h"

ssize_t readline(int fd, void* buffer, size_t n)
{
	ssize_t num_read;
	size_t total_read;
	char* internal_buffer;
	char character;

	if(n <= 0 || buffer == NULL)
	{
		errno = EINVAL;
		return -1;
	}

	internal_buffer = buffer;
	total_read = 0;

	for(;;)
	{
		num_read = read(fd, &character, 1);

		if(num_read == -1)
		{
			if(errno == EINTR)
				continue;
			else
				return -1;
		
		} 
		else if (num_read == 0)
		{
			if(total_read == 0)
				return 0;
			else
				break;
		}
		else 
		{
			if(total_read < n - 1)
			{
				total_read++;
				*internal_buffer++ = character;
			}

			if(character == '\n')
				break;
		}
	}

	*internal_buffer = '\0';
	return total_read;
}
