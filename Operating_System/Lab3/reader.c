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
    int shm_id, i;
    key_t key;
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

    // 获取共享内存ID
    shm_id = shmget(key, 0, 0);
    if (shm_id == -1)
    {
        perror("shmget error");
        return -1;
    }
    printf("shm_id=%d\n", shm_id);

    // 映射共享内存
    school = (student *)shmat(shm_id, NULL, 0);
    for (i = 0; i < 3; i++)
    {
        printf("student's id:%d\n", (*(school + i)).id);
        printf("student's name:%s\n", (*(school + i)).name);
    }

    // 解除映射
    if (shmdt(school) == -1)
    {
        perror("detach error");
        return -1;
    }

    shmctl(shm_id, IPC_RMID, 0); // 删除共享内存

    return 0;
}