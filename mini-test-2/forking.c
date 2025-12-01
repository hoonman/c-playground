#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int status, pid, pid2;
    pid = fork();
    if (pid == 0)
    {
        printf("1");
    }
    else
    {
        pid2 = fork();
        if (pid2 == 0)
        {
            printf("2");
            return 0;
        }
        else
        {
            printf("3");
            waitpid(pid2, &status, 0);
            printf("4");
        }
        waitpid(pid, &status, 0);
        printf("5");
    }
    return 0;
}