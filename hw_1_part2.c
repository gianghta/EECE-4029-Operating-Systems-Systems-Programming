#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Invalid statement. Usage: hw_1_part2 file_name\n");
        return 0;
    }

    char *fileName = argv[1];

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

    if (pid1 != 0 && pid2 != 0 && pid3 != 0)
    {
        printf("This parent process with pid %d\n", getpid());
        wait(NULL);
        wait(NULL);
        wait(NULL);
        printf("MAIN PROCESS TERMINATES. \n");
    }
    else
    {
        if (pid1 == 0)
        {
            execl("/bin/ls", "ls", "-l", NULL);
        }

        if (pid1 != 0 && pid2 == 0)
        {
            execl("/bin/ps", "ps", "-ef", NULL);
        }

        if (pid1 != 0 && pid2 != 0 && pid3 == 0)
        {
            execl("/bin/cat","cat", fileName, NULL);
        }
    }

    return 0;
}