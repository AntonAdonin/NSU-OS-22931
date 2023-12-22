#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

typedef struct table {
	int* lenStr;
	int* seek;
	int len;
	int currentLen;
}Table;

Table table = { 0, 0, 0 };
int file = 0;

void a_alarm(int t) {
	char buf[100];
	lseek(file, 0, SEEK_SET);
	read(file, buf, 100);
	printf("%s", buf);
	exit(0);
}

void initTable(Table* table) {
	table->len = 3;
	table->currentLen = 0;
	table->lenStr = (int*)calloc(table->len, sizeof(int));
	table->seek = (int*)calloc(table->len, sizeof(int));
}

void checkMemory(Table* table) {
	if (table->currentLen == table->len) {
		table->len *= 2;
		table->lenStr = (int*)realloc(table->lenStr, table->len * sizeof(int));
		table->seek = (int*)realloc(table->seek, table->len * sizeof(int));
	}
}

Table doTable(Table table, int file) {
	char letter;
	while (read(file, &letter, 1) == 1) {
		checkMemory(&table);
		if (letter == '\n' || file == 0) {
			if (table.currentLen == 0) {
				table.seek[table.currentLen] = 0;
			}
			else { table.seek[table.currentLen] = table.lenStr[table.currentLen - 1] + table.seek[table.currentLen - 1] + 1; }
			table.currentLen++;
		}
		else {
			table.lenStr[table.currentLen]++;
		}
	}
	table.seek[table.currentLen] = table.lenStr[table.currentLen - 1] + table.seek[table.currentLen - 1] + 1;
	return table;
}

int main(int argc, char* argv[]) {

	initTable(&table);

	int ret;

	signal(SIGALRM, a_alarm);

	if (argc < 2) {
		printf("The second argument is missing: file name\n");
	}

	file = open(argv[1], O_RDONLY);
	if (file == -1) {
		printf("Can't open file\n");
		exit(0);
	}

	table = doTable(table, file);

	int number;
	char* buf = (char*)malloc(sizeof(char) * 100);

	while (1) {
		alarm(5);
		scanf("%d", &number);
		if (number == 0) break;
		if (0 < number && number <= table.currentLen + 1) {
			lseek(file, table.seek[number - 1], SEEK_SET);
			read(file, buf, table.lenStr[number - 1] * sizeof(char));
			printf("%s\n", buf);
		}
		else printf("invalid row index\n");
	}
	free(buf);
	close(file);
	return 0;
}