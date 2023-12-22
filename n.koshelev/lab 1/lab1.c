#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <ulimit.h>

extern char** environ;

int main(int argc, char* argv[]) {
    char options[] = "cipsdvuU:C:V:";
    int c;
    struct rlimit rlm;

    printf("argc equals %d\n\n", argc);
    while ((c = getopt(argc, argv, options)) != EOF) {
        switch (c) {
            //      -i �������� �������� � ����������� �������������� ������������ � ������.
        case 'i':
            printf("Real user id = %d    ", getuid());
            printf("Effective user id = %d\n", geteuid());
            printf("Real group id = %d   ", getgid());
            printf("Effective group id = %d\n------------------------\n", getegid());
            break;

            //      -s ������� ���������� ������� ������. ���������: ������ setpgid(2).
        case 's':
            printf("Process is becoming the leader of the group\n------------------------\n");
            setpgid(0, getgid());
            break;

            //      -p �������� �������������� ��������, ��������-�������� � ������ ���������.
        case 'p':
            printf("Process id = %d\n", getpid());
            printf("Parent process id = %d\n", getppid());
            printf("Group process id = %d\n------------------------\n", getpgrp());
            break;
            //      -u  �������� �������� ulimit
        case 'u':
            printf("ulimit is %ld\n------------------------\n", ulimit(UL_GETFSIZE));
            break;

            //      -Unew_ulimit  �������� �������� ulimit. ���������: ������ atol(3C) �� �������� ����������� strtol(3C)
        case 'U':
            if (ulimit(UL_SETFSIZE, atol(optarg)) == -1) {
                perror("Wasn't able to change file limit\n");
                return 1;
            }
            printf("ulimit changed to %ld\n------------------------\n", atol(optarg));
            break;

            //      -c �������� ������ � ������ core-�����, ������� ����� ���� ������.
        case 'c':
            getrlimit(RLIMIT_FSIZE, &rlm);
            printf("Current core file size limit = %ld\n", rlm.rlim_cur);
            printf("Maximum allowed = %4ld\n------------------------\n", rlm.rlim_max);
            break;

            //      -Csize  �������� ������ core-�����
        case 'C':
            getrlimit(RLIMIT_FSIZE, &rlm);
            rlm.rlim_cur = atol(optarg);
            if (!setrlimit(RLIMIT_FSIZE, &rlm))
                printf("Succsesfully changed to %ld\n------------------------\n", rlm.rlim_cur);
            else {
                perror("strrlimit() error");
                return 1;
            }
            break;

            //      -d �������� ������� ������� ����������
        case 'd': {
            char* pathname = getenv("PWD");
            if (NULL == pathname) {
                perror("failed to get the current directory\n");
            }
            else {
                printf("current directory: %s\n", pathname);
            }
            break;
        }

                //      -v ������������� ���������� ����� � �� ��������
        case 'v':
            printf("Current envs\n");
            for (int i = 0; environ[i] != 0; i++)
                printf("%d    %s\n", i, environ[i]);
            printf("\n------------------------\n");
            break;

            //      -Vname=value ������ ����� ���������� � ����� ��� �������� �������� ������������ ����������.
        case 'V':
            putenv(optarg);
            printf("Added new env %s\n------------------------\n", optarg);
            break;
        }
        return 0;
    }
}