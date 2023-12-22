FLAGS = -Wall -Werror -Wextra
CC = gcc

all:
	$(CC) server.c $(FLAGS) -o server
	$(CC) client.c $(FLAGS) -o client

run_server:
	gnome-terminal -e "bash -c './server; read -p \"Press Enter to exit...\"'"

run_client:
	gnome-terminal -e "bash -c './client; read -p \"Press Enter to exit...\"'"


stop:
	pkill -f gnome-terminal
