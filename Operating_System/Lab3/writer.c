#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct student
{
    int id;
    char name[8];
} student;

int main(int argc, char **argv)
{
    int shm_id;
    int i = 0;
    key_t key;
    char buffer[BUFSIZ];
    student *school;
    char pathname[30];

    // 根据路径名和项目id生成key
    strcpy(pathname, "/tmp");
    key = ftok(pathname, 0x03);
    if (key == -1)
    {
        perror("ftok error");
        return -1;
    }
    printf("key=%d\n", key);

    // 创建共享内存
    shm_id = shmget(key, 4096, IPC_CREAT | IPC_EXCL | 0600); // IPC_CREAT | IPC_EXCL时, 如果没有该块共享内存，则创建，并返回共享内存ID
    if (shm_id == -1)
    {
        perror("shmgct error");
        return -1;
    }
    printf("shm_id=%d\n", shm_id);

    // 映射共享内存
    school = (student *)shmat(shm_id, NULL, 0);
    for (i = 0; i < 3; i++)
    {
        printf("please input student name: ");
        fgets(buffer, BUFSIZ, stdin);
        (school + i)->id = 0 + i;
        strncpy((school + i)->name, buffer, 5);
    }

    shmdt(school); // 解除映射

    return 0;
}