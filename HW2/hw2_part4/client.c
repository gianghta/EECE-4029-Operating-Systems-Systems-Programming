#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>

#define STOP "Stop\n"
#define MAXCHAR 1000

const int SHM_SIZE = 255;
const char FILE_NAME[] = "test-file.c";

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Invalid statement. Usage: hw2_part4 [file_name]\n");
        return 0;
    }

    key_t key;
    int shmid;
    char *data;
    char *fileName;
    char str[1000];
    FILE *fptr;

    int counter = 0;
    int n;

    /* open text file */
    fileName = argv[1];
    fptr = fopen(fileName, "r");

    /* make the key: */
    key = ftok(FILE_NAME, 1);

    /* connect to (and possibly create) the segment: */
    shmid = shmget(key, SHM_SIZE + 1, 0666 | IPC_CREAT);
    
    /* attach to the segment to get a pointer to it: */
    data = (char*)shmat(shmid, NULL, 0);

    data[SHM_SIZE] = 0;

    while (fgets(str, MAXCHAR, fptr) != NULL) {
        while(data[SHM_SIZE] == 1);
        while(str[counter]!='\0') 
        {
            if(str[counter]>='a' && str[counter]<='z')
            {
                str[counter] -= 32;
            }
            data[counter] = str[counter];
            counter++;
        }
        data[counter] = '\0';
        counter = 0;
        data[SHM_SIZE] = 1;
        sleep(1);
        /* Write into shared memory */
    }
    while(data[SHM_SIZE] == 1);
    memcpy(data, STOP, strlen(STOP) + 1);
    data[SHM_SIZE] = 1;
    /* close file */
    fclose(fptr);

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }
    
    return 0;
}
