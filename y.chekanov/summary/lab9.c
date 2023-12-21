// 9. �������� ���� ���������
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();  // �������� �����������
    if (pid == -1)  // ���� fork �� ������, ���������� -1
        return 1;
    if (pid == 0) {  // ���� ������� ����� �������������� 0, �� ����������� cat ��� task.c
        execlp("cat", "cat", "lab9.c", NULL);  // ������ �������� - ��, ��� ���� �����������
        return 1;
    }
    else {  // �����, ���� ������� �� �������� ������������, �� �� ���� ��������� ����������
        if (wait(NULL) != -1)  // � ������� wait
            printf("\nSubprocess pid: %d finished\n", pid);
    }

    return 0;
}
