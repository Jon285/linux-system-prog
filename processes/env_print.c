#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main(void)
{
	for(char** ep = environ; *ep != NULL; ep++)
		puts(*ep);
	return 0;
}
