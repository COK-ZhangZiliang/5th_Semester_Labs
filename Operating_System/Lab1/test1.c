#include <stdio.h>
/**
 * @file test1.c
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief fork, wait, exit, getpid系统调用实例
 * @version 0.1
 * @date 2023-11-20
 */
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    int child_pid, parent_pid, pid;
    int status;
    if ((pid = fork()) > 0)
    {
        // 父进程处理过程
        child_pid = wait(&status);
        printf("I am the parent process.\n");

        if (pid == child_pid)
            printf("My child process has finished, and its process id = %d, which is equal to fork() retrun value.\n", child_pid);
        
        printf("My child process's return code is %d.\n", status);
    }

    else if (pid == 0)
    {
        // 子进程处理过程
        printf("I am the child process.\n");

        child_pid = getpid();
        printf("My process id = %d.\n", child_pid);

        exit(0);
    }

    else
    {
        printf("fork error\n");
        exit(1);
    }

    return 0;
}