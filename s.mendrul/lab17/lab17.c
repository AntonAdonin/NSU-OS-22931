#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define MAX_LENGTH 40

#define CTRL_U 21
#define CTRL_W 23
#define CTRL_D 4

int main(){
	struct termios old_settings, new_settings;
	char input[MAX_LENGTH];
	int index = 0;

	tcgetattr(STDIN_FILENO, &old_settings);
	new_settings = old_settings;
	new_settings.c_lflag &= ~(ECHO| ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

	while(1){
		char c = getchar();
		if(c == '\b'){
			if(index > 0){
				index--;
				input[index] = '\0';
			}
		}else if(c == CTRL_U){//CTRL-U
			index = 0;
			input[index] = '\0';
		} else if(c == CTRL_W){//CTRL-W
			while (index > 0 && input[index - 1] == ' '){
				index--;
				input[index] = '\0';
			}
			while(index > 0 && input[index - 1] != ' '){
				index--;
				input[index] = '\0';
			}
		}else if(c == CTRL_D){//CTRL-D
			if(index == 0){
				break;
			}
		}else{
			if(index < MAX_LENGTH-1){
				input[index] = c;
				index++;
				input[index] = '\0';
			}
			else{
				printf("\n");
				index = 0;
				input[index] = '\0';
			}
		}
		printf("%s\r", input);
		fflush(stdout);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
	return 0;
}
