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
    key_t key = ftok(".", 'a');
    int msgid = msgget(key, IPC_CREAT | 0666); // get the message queue id
    if (msgid == -1)
    {
        perror("msgget");
        exit(1);
    }

    struct message msg;

    while (1)
    {
        if (msgrcv(msgid, &msg, MSG_SIZE, 0, 0) == -1)
        {
            perror("msgrcv");
            exit(1);
        }
        printf("Received: %s\n", msg.mtext);

        // Exit if the user enters "exit"
        if (strncmp(msg.mtext, "exit", 4) == 0)
        {
            break;
        }
    }

    // Remove the message queue
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }

    return 0;
}
