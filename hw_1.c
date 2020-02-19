#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

bool isNumber(char* a)
{
    int i = 0;
    if(a[0] == '-')
    {
        i = 1;
    }

    for(; a[i] != 0; i++)
    {
        if(isdigit(a[i]))
        {
            return false;
        }
    }
    return true;
}

int main(int argc, char* argv[])
{
    if(argc != 2 || isNumber(argv[1])) {
        printf("Invalid statement. Usage: hw_1 number1\n");
        return 0;
    }
    int n = atoi(argv[1]);

    int pid1 = 0, pid2 = 0, pid3 = 0;

    pid1 = fork();

    if (pid1 != 0)
    {
        pid2 = fork();
    }
    if (pid1 != 0 && pid2 != 0)
    {
        pid3 = fork();
    }


    for (int i = 0; i < n; i++)
    {
        if (pid1 != 0 && pid2 != 0 && pid3 != 0)
        {
            printf("This parent process with pid %d\n", getpid());
            sleep(2);
        }
        else {
            printf("This is a child process, my PID is %d, my parent PID is %d\n", getpid(), getppid());
            sleep(2);
        }
    }

    return 0;
}