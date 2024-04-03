#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[])
{
    int n = atoi(argv[argc-1]);
    int result = (int)n*2;
    printf("Twice: Current process id: %d, Current result: %d\n", getpid(), result);
    if(argc>2)
    {
        sprintf(argv[argc-1], "%d", result);
        execvp(argv[1], argv + 1);
    }
    return 0;
}