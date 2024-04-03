#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> 
#include<sys/wait.h>

int main(int argc, char* argv[])
{
    char s[] = "Hello World";
    int pid = 0;
    int t = 0;
    for(int i=0; i<11; i++)
    {
        if(pid == 0)
        {
            t = rand()%4 + 1;
            sleep(t);       
            pid = fork();
            if(pid == 0)
            {
                printf("%d : %d : %c\n", getpid(), getppid(), s[i]);
            }
            wait(NULL);
        }
    }
    return 0;
}
