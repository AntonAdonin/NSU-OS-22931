#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


struct termios term;
struct termios newTerm;

typedef struct Line {
	char* str;
	int len;
}Line;

void initLine(Line* line) {
	line->str = (char*)malloc(sizeof(char) * 41);
	line->len = 0;
	line->str[0] = '\0';
}

int main(int argc, char* argv[]) {
	tcgetattr(0, &term);
	newTerm = term;
	newTerm.c_lflag &= ~(ECHO | ICANON);
	newTerm.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &newTerm);
	printf("%d\n", tcgetattr(0, &newTerm));

	Line line;
	initLine(&line);

	char symbol;
	while (read(0, &symbol, 1)) {
		if (symbol == CEOF && line.str[0] == '\0') {
			break;
		}
		else if (line.len > 0 && symbol == CERASE) {
			
			line.len--;
			printf("\033[D\033[K");
			line.str[line.len] = '\0';
			fflush(stdout);
		}
		else if (line.len > 0 && symbol == CKILL) {
			line.len = 0;
			line.str[0] = '\0';
			printf("\33[2K\r");
			fflush(stdout);
		}
		else if (symbol == CWERASE) {
			int count = 0;
			int l = line.len;
			while (isspace(line.str[line.len-1]) && line.len > 0) {
				count++;
				line.len--;
			}
			while (!isspace(line.str[line.len - 1]) && line.len > 0) {
				count++;
				line.len--;
			}
			
			printf("\033[%dD\033[K", count);
			fflush(stdout);
			line.str[l - count+1] = '\0';	
		}
		else {
			if (isprint(symbol) == 0) {
				printf("\a");
				fflush(stdout);
			}
			else {
				if (line.len == 40) {
					printf("\n");
					line.len = 0;
					line.str[0] = '\0';
				}
				line.str[line.len] = symbol;
				line.len++;
				line.str[line.len] = '\0';
				write(1, &symbol, 1);
			}
		}
	}

	tcsetattr(0, TCSANOW, &term);
}