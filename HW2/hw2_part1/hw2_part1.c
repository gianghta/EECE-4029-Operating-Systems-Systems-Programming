#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    if(argc != 3) {
        printf("Invalid statement. Usage: hw_2_part1 [program] file_name\n");
        return 0;
    }

    char *fileName = argv[2];
    char *cmd = argv[1];

    int pid = 0;

    pid = fork();
    if(pid == 0)
    {
        int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        dup2(fd, STDOUT_FILENO);
        execl (cmd, cmd, (char *)0);
        close(fd);
    }

    return 0;
}