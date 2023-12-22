#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#define   BACKSPACE   write(1, "\b \b", 3)

int main(int argc, char* argv[])
{
	char line[512], ch;
	struct termios tty, buftty;

	if (!isatty(0))
	{
		perror(argv[0]);
		exit(1);
	}
	if (tcgetattr(0, &tty) == -1)
	{
		perror(argv[0]);
		exit(2);
	}
	
	buftty = tty;
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 1;
	tty.c_lflag &= ~(ISIG | ICANON | ECHO);
	if (tcsetattr(0, TCSANOW, &tty) == -1)
	{
		perror(argv[0]);
		exit(3);
	}

	int pos = 0;
	while (read(0, &ch, 1) > 0)
	{
		if (ch == CEOT)
			if (!pos) break;
			else if (ch == tty.c_cc[VERASE])
				if (pos > 0)
				{
					BACKSPACE;
					pos--;
				}
				else if (ch == tty.c_cc[VKILL])
					while (pos > 0)
					{
						BACKSPACE;
						pos--;
					}
				else if (ch == CWERASE)
				{
					while (pos > 0 && isspace(line[pos - 1]))
					{
						BACKSPACE;
						pos--;
					}
					while (pos > 0 && !isspace(line[pos - 1]))
					{
						BACKSPACE;
						pos--; 
					}
				}
				else if (ch == '\n')
				{
					write(1, &ch, 1);
					pos = 0;
				}
				else if (!isprint(ch)) write(1, "\07", 1);
				else
				{
					write(1, &ch, 1);
					line[pos++] = ch;
				}
		if (pos >= 40 && !isspace(ch))
		{
			int bufpos = pos;
			while (pos > 0 && !isspace(line[pos - 1]))
				pos--;
			if (pos > 0)
			{
				int newpos = 0;
				for (int i = pos; i < bufpos; i++)
				{
					BACKSPACE;
					line[newpos++] = line[i];
				}
				pos = newpos;
				write(1, "\n", 1);
				for (int i = 0; i < pos; i++)
					write(1, &line[i], 1);

			}
			else write(1, "\n", 1);
		}
	}

	tcsetattr(0, TCSANOW, &buftty);

	return 0;
}