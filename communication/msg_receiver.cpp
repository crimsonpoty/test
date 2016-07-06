#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

struct msgbuf2
{
	long msgtype;
	int  seq;
};

int main(/*int argc, char **argv*/)
{
	key_t key_id;
	struct msgbuf2 mybuf;
	int msgtype;//전역변수로 빼야 됩니다.

	// 아규먼트가 있을경우 msgtype 가 3인 메시지를 받아오고(홀수)
	// 아규먼트가 없을경우 msgtype 가 4인 메시지를 받아온다(짝수)

	msgtype = 4;

	key_id = msgget(1234, IPC_CREAT|0666);
	if (key_id < 0)
	{
		perror("msgget error : ");
		exit(0);
	}
	while(1)
	{
		if (msgrcv( key_id, (void *)&mybuf, sizeof(mybuf), msgtype, 0) == -1)
		{
			perror("msgrcv error : ");
			exit(0);
		}
		printf("%d\n", mybuf.seq);
	}
	exit(0);
}
