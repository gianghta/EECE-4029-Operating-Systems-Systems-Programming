#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    const int MAXLINE = 80;
    char inputList[MAXLINE];
    char outputList[MAXLINE];

    FILE *fp = popen("find . -name \\*.c", "r");
    FILE *fout = popen("tr [a-z] [A-Z]", "w");

    while (fgets(inputList, MAXLINE, fp) != NULL)
    {
        printf("%s", inputList);
        fputs(inputList, fout);
    }
    pclose(fp);
    pclose(fout);

    return 0;
}