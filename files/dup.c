#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

int _dup(int fd)
{
	return fcntl(fd, F_DUPFD);
}

int _dup2(int oldfd, int newfd)
{
	if(fcntl(oldfd, F_GETFL) == -1)
	{
		errno = EBADF;
		return -1;
	}

	if(oldfd == newfd)
		return newfd;
	
	if(fcntl(newfd, F_GETFL) != -1)
		close(newfd);
	
	fcntl(oldfd, F_DUPFD, newfd);
	return newfd;
}
