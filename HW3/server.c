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
#define RECEIVED "Received\n"

const int SHM_SIZE = 255;
const char FILE_NAME[] = "hw2_part1.c";

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
    int semaid;
	union semun arg;
	struct sembuf wait[1];
	struct sembuf signal[1];


    /* make the key: */
    key = ftok(FILE_NAME, 1);

    /* connect to (and possibly create) the segment: */
    shmid = shmget(key, SHM_SIZE + 1, 0666 | IPC_CREAT);
    
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

    // Initialize semaphore to be 1
	arg.val = 1;
	semctl(semaid, 0, SETVAL, arg);

    /* read the segment from client program */
    data[0] = '\0';

    while(1)
    {   
        // Lock
		semop(semaid, wait, 1);

        printf("%s", data);
        if(strcmp(data, STOP) == 0)
        {
            break;
        }
        if(data != NULL)
        {
            memcpy(data, RECEIVED, strlen(RECEIVED) + 1);
        }
        // Unlock
		semop(semaid, signal, 1);
        sleep(1);
    }

    /* detach from the segment: */
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }

     // Remove semaphore
	semctl(semaid, 0, IPC_RMID, 0);

    shmdt(data);

    /* delete the IPC structure */
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}