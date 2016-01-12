#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <sys/stat.h> 

struct msgbuf1
{
    long msgtype;
    char mtext[256];
    char myname[16];
    int  seq;
};

int main()
{
    key_t key_id;
    int i;
    struct msgbuf1 mybuf, rcvbuf;

    key_id = msgget((key_t)1234, IPC_CREAT|0666);
    if (key_id == -1)
    {
        perror("msgget error : ");
        exit(0);
    }

    printf("Key is %d\n", key_id);

    memset(mybuf.mtext, 0x00, 256); 
    memset(mybuf.myname, 0x00, 16); 
    memcpy(mybuf.mtext, "hello world 4", 13);
    memcpy(mybuf.myname, "yundream", 8);
    mybuf.seq = 0;
    i = 0;

    while(1)
    {
	// 짝수일경우 메시지 타입이 4
	// 홀수일경우에는 메시지 타입이 3
	if (i % 2 == 0)
            mybuf.msgtype = 4;
        else 
            mybuf.msgtype = 3;
        mybuf.seq = i;

        // 메시지를 전송한다.  
        if (msgsnd( key_id, (void *)&mybuf, sizeof(struct msgbuf1), IPC_NOWAIT) == -1)
        {
            perror("msgsnd error : ");
            exit(0);
        } 
        printf("send %d\n", i);
        i++;
        sleep(1);
    }

    printf("%ld \n", rcvbuf.msgtype);
    printf("%s \n", rcvbuf.mtext);
    printf("%s \n", rcvbuf.myname);
    exit(0);
}
