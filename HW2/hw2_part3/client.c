#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define STOP "Stop\n"
#define MAXCHAR 1000
#define err(mess) { fprintf(stderr,"Error: %s.", mess); exit(1); }

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Invalid statement. Usage: hw_1_part2 file_name\n");
        return 0;
    }

    int fd;
    int counter = 0;

    char str[1000];
    char *fileName = argv[1];
    FILE *fptr;

    fptr = fopen(fileName, "r");
    if (fptr == NULL){
        printf("Could not open file %s",fileName);
        return 1;
    }


    mkfifo("fifo_x", 0666);
    if ( (fd = open("fifo_x", O_WRONLY)) < 0) {
        err("open")
    }

    while (fgets(str, MAXCHAR, fptr) != NULL) {
        while(str[counter]!='\0') 
        {
            if(str[counter]>='a' && str[counter]<='z')
            {
                str[counter] -= 32;
            }
            counter++;
        }
        counter = 0;

        if ( write(fd, str, strlen(str)) != strlen(str)) { 
            err("write\n");
        }
        sleep(1);
    }

    write(fd, STOP, 5);

    close(fd);
    fclose(fptr);
    return 0;
}