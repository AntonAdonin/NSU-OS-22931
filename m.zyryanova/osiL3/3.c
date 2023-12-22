#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {

	printf("real user id: %d\n", getuid());
	printf("effective user id: %d\n", geteuid());

	if (argc == 1) {
		printf("You didn't specify a file\n");
		exit(0);
	}

	FILE* file = fopen(argv[1], "r");
	if (file == NULL) {
		perror("I can't open file");
	}
	else {
		fclose(file);
	}

	setgid(getuid());

	FILE* file1 = fopen(argv[1], "r");

	if (file1 == NULL) {
		perror("I can't open file");
	}
	else {
		fclose(file1);
	}

	printf("real user id: %d\n", getuid());
	printf("effective user id: %d\n", geteuid());

	return 0;
}#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {

	printf("real user id: %d\n", getuid());
	printf("effective user id: %d\n", geteuid());

	if (argc == 1) {
		printf("You didn't specify a file\n");
		exit(0);
	}

	FILE* file = fopen(argv[1], "r");
	if (file == NULL) {
		perror("I can't open file");
	}
	else {
		fclose(file);
	}

	setgid(getuid());

	FILE* file1 = fopen(argv[1], "r");

	if (file1 == NULL) {
		perror("I can't open file");
	}
	else {
		fclose(file1);
	}

	printf("real user id: %d\n", getuid());
	printf("effective user id: %d\n", geteuid());

	return 0;
}