#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define FILE_PATH "./mmap_file"
#define STOP "Stop\n"
#define MAXCHAR 1000

const int SHM_SIZE = 255;

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Invalid statement. Usage: hw2_part5 [file_name]\n");
        return 0;
    }

    char *data;
    char *fileName;
    char str[1000];
    FILE *fptr;
    int map_fd;

    int counter = 0;
    int n;

    /* open text file */
    fileName = argv[1];
    fptr = fopen(fileName, "r");

    map_fd = open(FILE_PATH, O_RDWR);

	// create map file and write dummy bytes
	data = mmap(NULL, SHM_SIZE + 1, PROT_READ | PROT_WRITE, MAP_SHARED, map_fd, 0);

    // Set flag to signal server
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

    // Delete mmap
	munmap(data, SHM_SIZE + 1);

	// Close map file
	close(map_fd);
    
    return 0;
}
