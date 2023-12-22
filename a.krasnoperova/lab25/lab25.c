#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <osfcn.h>
#include <ctype.h>

int main(int argc, char** argv)
{
	int pid, rtn, fd[2];
	static char* lines[3] = { "A bull is walking, swaying, sighing as he walks:\n", "Oh, the plank's running out!\n", "Now I'm gonna fall!!\n" };
	char input[1000];

	if (pipe(fd) == -1)
	{
		perror(argv[0]);
		return 1;
	}
	if ((pid = fork()) > 0) 
	{
		close(fd[0]);
		for (int i = 0; i < 3; i++)
			write(fd[1], lines[i], strlen(lines[i]));
		close(fd[1]);
	}
	else if (!pid) 
	{
		close(fd[1]);
		while ((rtn = read(fd[0], input, 1000)) > 0)
		{
			for (int i = 0; i < rtn; i++)
				if (islower(input[i]))
					input[i] = toupper(input[i]);
			write(1, input, rtn);
		}
		close(fd[0]);

	}
	else
	{
		perror(argv[0]);
		return 2;
	}
	return 0;
}