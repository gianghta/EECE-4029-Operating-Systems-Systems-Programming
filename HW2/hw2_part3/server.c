#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STOP "Stop\n"
#define MAXCHAR 1000
#define err(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }

int find_stop(char *str1, char *str2, int len)
{
    for (int i = 0; i < len; i++)
    {
        if(str1[i] != str2[i])
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    int fd_pipe;
    int n;

    char str[MAXCHAR];

    if ( (fd_pipe = open("fifo_x", O_RDONLY)) < 0) {
        err("open");
    }

    while(1)
    {
        n = read(fd_pipe, str, MAXCHAR);
        if(n > 0)
        {
            str[n] = '\0';
            printf("%s",str);
            if(find_stop(str, STOP, n) && n == 5)
            {
                break;
            }
        }
    }

    close(fd_pipe);

    execl("/bin/rm", "rm", "-f", "fifo_x", NULL);
    return 0;
}