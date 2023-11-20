/**
 * @file test3.c
 * @author zhang ziliang (ziliangzhang@bupt.edu.cn)
 * @brief signal, kill, clone系统调用实例
 * @version 0.1
 * @date 2023-11-20
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>

#define MY_SIG 15

int global_var = 0;

void func()
{
    printf("It is signal %d processing function!\n", MY_SIG);
}

void childProcessLogic()
{
    sleep(3); // 增加延迟
    printf("Child process - PID: %d, received a signal from my parent.\n", getpid());
    global_var++;
    printf("global_var = %d.\n", global_var);
    printf("Child process exiting.\n");
    exit(0);
}

void childThreadLogic()
{
    global_var++;
    printf("Child thread - PID: %d, global_var = %d.\n", getpid(), global_var);
    sleep(3); // 增加延迟
    printf("Child thread exiting.\n");
    exit(0);
}

int main()
{
    pid_t pid;
    int status;

    // 注册信号处理函数
    signal(MY_SIG, func);

    // 使用 fork() 创建子进程
    pid = fork();

    if (pid == 0)
    {
        childProcessLogic();
    }

    else if (pid > 0)
    {
        printf("Parent process - PID: %d.\n", getpid());

        printf("Forked child process - PID: %d.\n", pid);

        // 使用 kill() 命令结束子进程
        int result = kill(pid, MY_SIG);

        if (result == 0)
        {
            printf("Successfully sent MY_SIG signal to child process - PID: %d.\n", pid);
        }
        else
        {
            perror("kill");
        }

        // 使用 wait() 等待子进程结束
        wait(&status);
        printf("Child process returned with status: %d, global_var = %d.\n", WEXITSTATUS(status), global_var);

        // 使用 clone() 创建子进程/线程
        // mask 参数的取值：
        // CLONE_VM：子进程与父进程运行于相同的内存空间中
        // CLONE_VFORK：父进程被挂起
        // CLONE_CHILD_SETTID：子进程与父进程共享相同的线程群
        int mask = CLONE_VM | CLONE_VFORK | CLONE_CHILD_SETTID;
        pid = clone(childThreadLogic, malloc(4096) + 4096, mask, NULL);
        if (pid == -1)
        {
            perror("clone");
        }
        else
        {
            printf("Cloned child process - PID: %d.\n", pid);
        }

        mask = CLONE_VFORK | CLONE_CHILD_SETTID;
        pid = clone(childThreadLogic, malloc(4096) + 4096, mask, NULL);
        if (pid == -1)
        {
            perror("clone");
        }
        else
        {
            printf("Cloned child process - PID: %d.\n", pid);
        }
        waitpid(pid, NULL, 0);

        printf("Parent process exiting, global_var = %d.\n", global_var);
    }

    else
    {
        printf("Fork failed.\n");
        return 1;
    }

    return 0;
}
