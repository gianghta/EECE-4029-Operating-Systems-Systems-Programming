#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define STOP "Stop\n"
#define MAXCHAR 1000

const int SHM_SIZE = 255;
const char FILE_NAME[] = "hw2_part1.c";

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
    int semaid;
	union semun arg;
	struct sembuf wait[1];
	struct sembuf signal[1];

    int counter = 0;
    int n;

    /* open text file */
    fileName = argv[1];
    fptr = fopen(fileName, "r");

    /* make the key: */
    key = ftok(FILE_NAME, 1);

    /* connect to (and possibly create) the segment: */
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    
    /* attach to the segment to get a pointer to it: */
    data = (char*)shmat(shmid, NULL, 0);

    /* Init sema wait and signal */
    wait[0].sem_num = 0;
	wait[0].sem_op = -1;
	wait[0].sem_flg = SEM_UNDO;
	signal[0].sem_num = 0;
	signal[0].sem_op = 1;
	signal[0].sem_flg = SEM_UNDO;

    // Get semaid
	semaid = semget(key, 1, 0666 | IPC_CREAT);

    while (fgets(str, MAXCHAR, fptr) != NULL) {
        // Lock
		semop(semaid, wait, 1);

        printf("%s", data);

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
    
        // Unlock
		semop(semaid, signal, 1);
        sleep(1);
        /* Write into shared memory */
    }
    // Lock
	semop(semaid, wait, 1);

    memcpy(data, STOP, strlen(STOP) + 1);

    /* close file */
    fclose(fptr);

    // Remove semaphore
	semctl(semaid, 0, IPC_RMID, 0);

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }
    
    return 0;
}
