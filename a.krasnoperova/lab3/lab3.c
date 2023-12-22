#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
int main(int argc, char** argv)
{
	FILE* file;
	uid_t u;
	if (argc < 2)
	{
		perror(stderr, "Invailid script\n");
		return 1;
	}
	printf("The real user id is %ld\n The effective user id is %ld\n", getuid(), geteuid());
	if ((file = fopen(argv[1], "r")) == NULL)
	{
		perror("The file can't be opened");
		return 2;
	}
	printf("The file is opened successfully\n");
	fclose(file);
	setuid(u = getuid());
	printf("setuid(%ld):\n   The real user id is %ld\n The effective user id%ld\n", u, getuid(), geteuid());
	if ((file = fopen(argv[1], "r")) == NULL)
	{
		perror("The file can't be opened");
		return 3;
	}
	printf("The file is opened successfully for the second time\n");
	fclose(file);
	return 0;
}
