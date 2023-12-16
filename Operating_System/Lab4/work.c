#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <errno.h>

/* 共享变量 */
int countA = 0; // 工作台中零件A的数量
int countB = 0; // 工作台中零件B的数量
int empty = 12; // 工作台中空位的数量

/* 互斥锁 */
pthread_mutex_t mutexA;     // 对countA的互斥锁
pthread_mutex_t mutexB;     // 对countB的互斥锁
pthread_mutex_t mutexEmpty; // 对empty的互斥锁同时控制对工作台的互斥访问

/* 信号量 */
sem_t suspendA;   // 用于A线程的挂起
sem_t suspendB;   // 用于B线程的挂起
sem_t suspendC;   // 用于C线程的挂起
sem_t isSuspendA; // 用于判断A线程是否被挂起
sem_t isSuspendB; // 用于判断B线程是否被挂起
sem_t isSuspendC; // 用于判断C线程是否被挂起

/* workerA生产两个零件A并放入工作台 */
void *workerA(void)
{
    while (1)
    {
        sleep(1);
        pthread_mutex_lock(&mutexA);
        pthread_mutex_lock(&mutexEmpty);
        if (empty >= 2 && countA <= 7) // 工作台有空位且能保证放下3个零件B
        {
            // 向工作台放入两个零件A
            empty -= 2;
            countA += 2;
            printf("====workerA====\n");
            printf("countA:%d empty=%d\n", countA, empty);
            int value;
            sem_getvalue(&isSuspendC, &value);
            if (value == 0 && (countA >= 4 && countB >= 3)) // 如果C线程被挂起且工作台中有足够的零件A和零件B
            {
                sem_post(&suspendC); // 唤醒C线程
            }
            pthread_mutex_unlock(&mutexEmpty);
            pthread_mutex_unlock(&mutexA);
        }
        else
        {
            pthread_mutex_unlock(&mutexEmpty);
            pthread_mutex_unlock(&mutexA);
            // 挂起A线程
            sem_wait(&isSuspendA);
            printf("A is suspended\n");
            sem_wait(&suspendA);
            printf("A is resumed\n");
            sem_post(&isSuspendA);
        }
    }
}

/* workerB生产一个零件B并放入工作台 */
void *workerB(void)
{
    while (1)
    {
        sleep(1);
        pthread_mutex_lock(&mutexB);
        pthread_mutex_lock(&mutexEmpty);
        if (empty >= 1 && countB <= 7) // 工作台有空位并能保证放下4个零件A
        {
            // 向工作台放入一个零件B
            empty -= 1;
            countB += 1;
            printf("====workerB====\n");
            printf("countB:%d empty=%d\n", countB, empty);
            int value;
            sem_getvalue(&isSuspendC, &value);
            if (value == 0 && (countA >= 4 && countB >= 3)) // 如果C线程被挂起且工作台中有足够的零件A和零件B
            {
                sem_post(&suspendC); // 唤醒C线程
            }
            pthread_mutex_unlock(&mutexEmpty);
            pthread_mutex_unlock(&mutexB);
        }
        else
        {
            pthread_mutex_unlock(&mutexEmpty);
            pthread_mutex_unlock(&mutexB);
            // 挂起B线程
            sem_wait(&isSuspendB);
            printf("B is suspended\n");
            sem_wait(&suspendB);
            printf("B is resumed\n");
            sem_post(&isSuspendB);
        }
    }
}

/* workerC从工作台中取出4个零件A和3个零件B */
void *workerC(void)
{
    while (1)
    {
        sleep(2);
        pthread_mutex_lock(&mutexA);
        pthread_mutex_lock(&mutexB);
        if (countA >= 4 && countB >= 3) // 工作台中有足够的零件A和零件B
        {
            pthread_mutex_lock(&mutexEmpty);
            // 从工作台中取出4个零件A和3个零件B
            countA -= 4;
            countB -= 3;
            empty += 7;
            printf("====workerC====\n");
            printf("countA:%d countB:%d empty=%d\n", countA, countB, empty);
            pthread_mutex_unlock(&mutexEmpty);
            pthread_mutex_unlock(&mutexB);
            pthread_mutex_unlock(&mutexA);
            int value1, value2;
            sem_getvalue(&isSuspendA, &value1);
            sem_getvalue(&isSuspendB, &value2);
            if (value1 == 0) // 如果A线程被挂起
            {
                sem_post(&suspendA); // 唤醒A线程
            }
            if (value2 == 0) // 如果B线程被挂起
            {
                sem_post(&suspendB); // 唤醒B线程
            }
        }
        else
        {
            pthread_mutex_unlock(&mutexB);
            pthread_mutex_unlock(&mutexA);
            // 挂起C线程
            sem_wait(&isSuspendC);
            printf("C is suspended\n");
            sem_wait(&suspendC);
            printf("C is resumed\n");
            sem_post(&isSuspendC);
        }
    }
}

int main(void)
{
    /* 线程的标识符 */
    pthread_t A, B, C;
    int ret = 0;

    /* 初始化互斥锁 */
    pthread_mutex_init(&mutexA, NULL);
    pthread_mutex_init(&mutexB, NULL);
    pthread_mutex_init(&mutexEmpty, NULL);

    /* 初始化信号量 */
    sem_init(&suspendA, 0, 0);
    sem_init(&suspendB, 0, 0);
    sem_init(&suspendC, 0, 0);
    sem_init(&isSuspendA, 0, 1);
    sem_init(&isSuspendB, 0, 1);
    sem_init(&isSuspendC, 0, 1);

    /* 分别创建线程ABC */
    ret = pthread_create(&A, NULL, (void *)workerA, NULL);
    if (ret != 0)
    {
        perror("workerA_create");
    }
    ret = pthread_create(&B, NULL, (void *)workerB, NULL);
    if (ret != 0)
    {
        perror("workerB_create");
    }
    ret = pthread_create(&C, NULL, (void *)workerC, NULL);
    if (ret != 0)
    {
        perror("workerB_create");
    }
    sleep(3);

    /* 等待线程ABC的结束 */
    pthread_join(A, NULL);
    pthread_join(B, NULL);
    pthread_join(C, NULL);

    return 0;
}