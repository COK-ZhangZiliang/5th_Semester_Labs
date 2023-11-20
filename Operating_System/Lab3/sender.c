#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE 256

struct message
{
    long mtype;
    char mtext[MSG_SIZE];
};

int main()
{
    key_t key = ftok(".", 'a'); // Generate a unique key
    int msgid = msgget(key, IPC_CREAT | 0666); // Create a message queue
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    struct message msg;
    msg.mtype = 1;
    char buffer[MSG_SIZE];

    while (1)
    {
        printf("Enter a message: ");
        fgets(buffer, MSG_SIZE, stdin);
        sprintf(msg.mtext, "%s", buffer);
        if (msgsnd(msgid, &msg, MSG_SIZE, 0) == -1)
        {
            perror("msgsnd");
            exit(1);
        }
        
        // Exit if the user enters "exit"
        if (strncmp(buffer, "exit", 4) == 0)
        {
            break;
        }
        sleep(1);
    }

    return 0;
}
