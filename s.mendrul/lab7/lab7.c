#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_LINE_LENGTH 256

int time_out = 0;

void sig_handler(int signo){
	if(signo == SIGALRM){
		time_out = 1;
	}
}

void print_line(char* file_data, int line_num, const off_t* offsets, const int* lengths, int num_lines) {
    if(line_num < 1 || line_num > num_lines){
    	perror("Неверный номер строки\n");
    	return;
    }

    off_t offset = offsets[line_num-1];
    int length = lengths[line_num-1];
    char buffer[MAX_LINE_LENGTH];
	snprintf(buffer, length+1, "%s", file_data + offset);
    printf("Строка:No.%d %s\n", line_num, buffer);
}

int main() {
    char filename[100];
    printf("Введите название файла: ");
   	scanf("%s", filename);
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        exit(1);
    }
	struct stat file_stat;
	if(fstat(fd, &file_stat) == -1){
		perror("Ошибка получения информации о файле");
		exit(1);
	}

	char* file_data = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if(file_data == MAP_FAILED){
		perror("Ошибка отображения файла в память");
		exit(1);
	}
	close(fd);
    
	char ch;
	int num_lines = 0;

	off_t offsets[MAX_LINE_LENGTH];
	int lengths[MAX_LINE_LENGTH];

	offsets[num_lines] = 0;
	lengths[num_lines] = 0;

	for(off_t i = 0; i < file_stat.st_size; i++){
		if(file_data[i] == '\n'){
			num_lines++;
			offsets[num_lines] = i+1;
			lengths[num_lines] = 0;
		} else{
			lengths[num_lines]++;
		}
	}

	int line_num;
	signal(SIGALRM, sig_handler);
	while(1){
		printf("Введите номер строки (0 для выхода): ");
		alarm(5);
		time_out = 0;
		scanf("%d", &line_num);

		if(time_out){
			printf("Время вышло....: \n");
			for(int i = 1; i <= num_lines; i++){
				print_line(file_data, i, offsets, lengths, num_lines);
			}
			break;
		}

		if(line_num == 0){
			break;
		}
		print_line(file_data, line_num, offsets, lengths, num_lines);
	}
	munmap(file_data, file_stat.st_size);
    return 0;
}

