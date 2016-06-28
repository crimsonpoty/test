/*
    C socket server example
*/
 
#include<stdio.h>
#include<stdlib.h>		//atoi
#include<string.h>		//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>		//write

// #define ECHO_SYSTEM 

#ifndef ECHO_SYSTEM
struct RealData
{
	int seq;
};

struct SendData
{
	int message_id;
	int message_size;
	RealData data;
};
#endif
 
int main(int argc , char *argv[])
{
	if(2 != argc) {
		printf("usage : %s [port]\n", argv[0]);
        return 1;
	}
	
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Server | Could not create socket\n");
    }
    printf("Server | Socket created\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(atoi(argv[1]));
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("Server | bind failed. Error");
        return 1;
    }
	printf("Server | bind done\n");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
	printf("Server | Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
	printf("Server | Connection accepted\n");
     
    //Receive a message from client
    while(1)
    {
		memset(client_message, 0x00, sizeof(client_message));
		read_size = recv(client_sock , client_message , sizeof(client_message), 0);
		
		if(read_size <= 0) {
			break;
		}
		
        //Send the message back to client
        write(client_sock, client_message, strlen(client_message));

#ifdef ECHO_SYSTEM
		printf("Server | Client Message: %s\n", client_message);
#else
		SendData recvData;
		memcpy(&recvData, (SendData *)&client_message, sizeof(SendData));
		printf("Server | Client Message: %d %d %d\n", recvData.message_id, recvData.message_size, recvData.data.seq);
#endif
    }
     
    if(read_size == 0)
    {
		printf("Server | Client disconnected\n");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
	
	close(socket_desc);
     
    return 0;
}