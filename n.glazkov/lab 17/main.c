#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 40

int main() {
    struct termios old_tio, new_tio;
    unsigned char c;
    char buffer[MAX_LENGTH + 1];
    int pos = 0;

    // Получаем текущие настройки терминала
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    // Отключаем канонический режим и эхо
    new_tio.c_lflag &=(~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    while (1) {
        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 4 && pos == 0) {
                // CTRL-D и курсор в начале строки
                break;
            } else if (c == 23) {
                // CTRL-W
		while (pos > 0 && buffer[pos - 1] != ' ') {
                    pos--;
		    write(STDOUT_FILENO, "\b \b", 3); 
                }
                while (pos > 0 && buffer[pos - 1] == ' ') {
                    pos--;
		    write(STDOUT_FILENO, "\b \b", 3); 
                }

            } else if (c == 127) {
                // Backspace
                if (pos > 0) {
                    pos--;
                    write(STDOUT_FILENO, "\b \b", 3); // Стираем символ на терминале
                }
            } else if (c == 21) {
                // CTRL-U
                while (pos > 0) {
                    pos--;
                    write(STDOUT_FILENO, "\b \b", 3); // Стираем символ на терминале
                }
            } else if (isprint(c)) {
                // Печатаемый символ
                if (pos < MAX_LENGTH) {
                    buffer[pos] = c;
                    pos++;
                    write(STDOUT_FILENO, &c, 1); // Печатаем символ на терминале
                }
            } else {
                // Непечатаемый символ
                write(STDOUT_FILENO, "\a", 1); // Звуковой сигнал
            }

            if (pos == MAX_LENGTH || c == '\n') {
                buffer[pos] = '\0';
                printf("\n%s\n", buffer);
                pos = 0;
            }
        }
    }

    // Восстанавливаем исходные настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return 0;
}
