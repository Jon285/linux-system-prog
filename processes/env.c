#include <stdlib.h>
#include <string.h>

extern char** environ;

int setenv_(const char* name, const char* value, int overwrite)
{
	if(!name || strlen(name) == 0 || strchr(name, '='))
		return -1;

	if(!getenv(name) || overwrite)
	{
		char* str = (char*)malloc(strlen(name) + strlen(value) + 1);

		if(!str)
			return -1;

		strcat(str, name);
		strcat(str, "=");
		strcat(str, value);

		if(!putenv(str))
			return -1;
		return 0;
	}
	return 0;
}

int unsetenv_(const char* name)
{
	if(!name || strlen(name) == 0 || strchr(name, '='))
		return -1;

	if(getenv(name))
	{
		for(char** env = environ; env != NULL; env++)
		{
			if(strstr(*env, name))
				*env = NULL;
		}
	}
	return 0;
}
