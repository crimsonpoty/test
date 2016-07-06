/*
	C ECHO client example using sockets
*/
#include<stdio.h>		// printf
#include<stdlib.h>		// atoi
#include<string.h>		// strlen

#include<sys/socket.h>	// socket
#include<arpa/inet.h>	// inet_addr
#include <unistd.h>		// close

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include <pthread.h>
#include <queue>


// #define ECHO_SYSTEM


struct RealData
{
	int seq;
};

struct SMessage
{
	long msgtype;
	RealData data;
};

struct SendData
{
	int message_id;
	int message_size;
	RealData data;
};

std::queue<SMessage> msgBuffer;
void *gathering_msg_from_queue(void * ptr);


int main(int argc , char *argv[])
{
	if(argc != 3) {
		printf("usage : %s [ip_addr] [port]\n", argv[0]);
		return 1;
	}

	pthread_t thread;
	int ret;

	// 스레드 시작
	ret = pthread_create(&thread, NULL, gathering_msg_from_queue, NULL);

	int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];

	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Client | Could not create socket\n");
	}
	printf("Client | Socket created\n");

	// server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_family = AF_INET;
	// server.sin_port = htons(8888);
	server.sin_port = htons(atoi(argv[2]));

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}

	printf("Client | Connected\n");

	//keep communicating with server
	while(1)
	{
#ifdef ECHO_SYSTEM
		printf("Enter message : ");
		scanf("%s" , message);

		if(!strncmp(message, "quit", 4)) {
			break;
		}
#else
		while(1) {
			if(msgBuffer.empty() == false) {
				break;
			}
			sleep(1);	//1초마다 msgBuffer 체크
		}
#endif

		//Send some data
#ifdef ECHO_SYSTEM
		int n = send(sock ,message , strlen(message) , 0);
#else
		SendData sendData;
		sendData.data.seq = msgBuffer.front().data.seq;
		sendData.message_id = 111;
		sendData.message_size = sizeof(RealData);		

		int n = send(sock, (char *)&sendData , sizeof(SendData) , 0);
		printf("Client | Send to Server :%d %d %d\n", sendData.message_id, sendData.message_size, sendData.data.seq);
#endif
		if(n < 0)
		{
			printf("Client | Send failed\n");
			return 1;
		}
#ifndef ECHO_SYSTEM
		msgBuffer.pop();
#endif

		//Receive a reply from the server
		n = recv(sock, server_reply , 2000 , 0);
		if(n < 0){
			printf("Client | recv failed\n");
			break;
		}

#ifdef ECHO_SYSTEM
		printf("Client | Server reply :");
		puts(server_reply);
#endif
	}

	close(sock);

	pthread_join(thread, NULL);
	printf("Thread returns: %d\n", ret);

	return 0;
}

// 메시지 큐에서 메시지가 수신되면 메시지버퍼(큐)에 집어 넣는 스레드
void *gathering_msg_from_queue(void * ptr)
{
	SMessage mBuf;

	key_t key_id = msgget(1234, IPC_CREAT|0666);

	if (key_id < 0)	{
		perror("msgget error : ");
		exit(0);
	}

	while(1)
	{
		memset(&mBuf, 0x00, sizeof(mBuf));
		if (msgrcv( key_id, (void *)&mBuf, sizeof(mBuf), 4, 0) == -1)
		{
			perror("msgrcv error : ");
			exit(0);
		}

		// queue에 넣을때는 mutex 처리 필요함
		msgBuffer.push(mBuf);
		printf("gathering_msg_from_queue | %d\n", mBuf.data.seq);
	}
}