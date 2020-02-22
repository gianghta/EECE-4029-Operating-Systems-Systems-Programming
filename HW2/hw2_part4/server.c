#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define STOP "Stop\n"

const int SHM_SIZE = 255;
const char FILE_NAME[] = "test-file.c";

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

int main(int argc, char *argv[])
{
    if(argc != 1) {
        printf("Invalid statement. Usage: hw2_part4\n");
        return 0;
    }

    key_t key;
    int shmid;
    char *data;

    /* make the key: */
    key = ftok(FILE_NAME, 1);

    /* connect to (and possibly create) the segment: */
    shmid = shmget(key, SHM_SIZE + 1, 0666 | IPC_CREAT);
    
    /* attach to the segment to get a pointer to it: */
    data = (char*)shmat(shmid, NULL, 0);

    data[SHM_SIZE] = 1;

    /* read the segment from client program */
    data[0] = '\0';

    while(1)
    {   
        while(data[SHM_SIZE] == 0);
        printf("%s", data);
        if(strcmp(data, STOP) == 0)
        {
            break;
        }
        data[SHM_SIZE] = 0;
    }

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

    /* delete the IPC structure */
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}