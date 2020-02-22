#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>

#define FILE_PATH "./mmap_file"
#define STOP "Stop\n"

const int SHM_SIZE = 255;

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
        printf("Invalid statement. Usage: hw2_part5\n");
        return 0;
    }

    char *data;
    int map_fd;


    // create map file and write dummy bytes
	map_fd = open(FILE_PATH, O_RDWR | O_CREAT | O_TRUNC, 0666);
	for(int i = 0; i < SHM_SIZE + 1; i++) {
		write(map_fd, "\0", 1);
	}

	// Create a mmap between data and map_fd
	data = mmap(NULL, SHM_SIZE + 1, PROT_READ | PROT_WRITE, MAP_SHARED, map_fd, 0);

    // set flag to signal client
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

    // Delete mmap
	munmap(data, SHM_SIZE + 1);

	// Close map file
	close(map_fd);

	// Remove map file
	execl("/bin/rm", "rm", "-f", FILE_PATH, NULL);

    
    return 0;
}