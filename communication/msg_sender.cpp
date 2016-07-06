#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

struct SMessage
{
	long msgtype;
	int  seq;
};

int main()
{
	key_t key_id;
	int i;
	struct SMessage mBuf, rcvBuf;

	key_id = msgget((key_t)1234, IPC_CREAT|0666);
	if (key_id == -1)
	{
		perror("msgget error : ");
		exit(0);
	}

	printf("Key is %d\n", key_id);

	mBuf.seq = 0;
	i = 0;

	while(1)
	{
		mBuf.msgtype = 4;	// 메시지 타입이 4
		mBuf.seq = i;

		// 메시지를 전송한다.
		if (msgsnd( key_id, (void *)&mBuf, sizeof(struct SMessage), IPC_NOWAIT) == -1)
		{
			perror("msgsnd error : ");
			exit(0);
		}
		printf("send %d\n", i);
		i++;
		sleep(5);
	}

	printf("%ld \n", rcvBuf.msgtype);
	exit(0);
}
