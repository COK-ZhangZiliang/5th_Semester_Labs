/**
 * @file test2.c
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief exec系列系统调用实例
 * @version 0.1
 * @date 2023-11-20
 */
#include <unistd.h>
int main(int arge, char *argv[])
{
    char *envp[] = {"PATH=/tmp", "USER=ziliangzhang", "STATUS = testing", NULL};
    char *argv_execv[] = {"echo", "excuted by execv", NULL};
    char *argv_execvp[] = {"echo", "excuted by execvp", NULL};
    char *argv_execve[] = {"env", NULL};
    if (fork() == 0)
    {
        if (execl("/bin/echo", "echo", "executed by execl", NULL) < 0)
            perror("Err on execl");
        exit(0);
    }
    if (fork() == 0)
    {
        if (execlp("echo", "echo", "executed by execlp", NULL) < 0)
            perror("Err on execlp");
        exit(0);
    }
    if (fork() == 0)
    {
        if (execle("/usr/bin/env", "env", NULL, envp) < 0)
            perror("Err on execle");
        exit(0);
    }
    if (fork() == 0)
    {
        if (execv("/bin/echo", argv_execv) < 0)
            perror("Err on execv");
        exit(0);
    }
    if (fork() == 0)
    {
        if (execvp("echo", argv_execvp) < 0)
            perror("Err on execvp");
        exit(0);
    }
    if (fork() == 0)
    {
        if (execve("/usr/bin/env", argv_execve, envp) < 0)
            perror("Err on execve");
        exit(0);
    }
    
    return 0;
}
