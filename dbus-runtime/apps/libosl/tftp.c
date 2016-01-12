/*
 * tftp.c
 *
 *  Created on: 2011. 10. 11.
 *      Author: buttonfly
 */


//#define UDP_PROTO_TFTP 		69

#define TFTP_BLOCKSIZE 		512

#define TFTP_OP_RRQ		1
#define TFTP_OP_WRQ		2
#define TFTP_OP_DATA		3
#define TFTP_OP_ACK		4
#define TFTP_OP_ERROR		5

#define TFTP_ERR_DISKFULL	3

#define TFTP_MAX_RETRIES	8

#define TFTP_RRQ_TIMEOUT	1	/* seconds */
#define TFTP_RECV_TIMEOUT	10000	/* 10 seconds */

#include <sys/socket.h>
#include <netinet/in.h>
#include "ioutil.h"
#include "stringtokenizer.h"
#include "osldbg.h"
#define _out_

//#define ERR
//#define H_


#include "tftp.h"
#include <string.h>

typedef struct
{
	int sockfd;
	struct sockaddr_in serveraddr;
	int client_addr_len;
	tftp_open_settings settings;
	char ip[32];
	unsigned short port;
	char filename[512];
} tftp_t;

typedef int (*decode_func)(tftp* context, ioutil io);

struct op_func_map
{
	unsigned short opcode;
	decode_func fp;
};

#define DECODE_FUNC(name) static int name(tftp* t, ioutil io)

DECODE_FUNC(decode_op_data);
DECODE_FUNC(decode_op_error);


static int __tftp_parse_url(const char* url, _out_ char* ip, _out_ unsigned short* port, _out_ char* file);

static const struct op_func_map s_op_func [] = {
		{0, NULL},
		{TFTP_OP_RRQ, NULL},
		{TFTP_OP_WRQ, NULL},
		{TFTP_OP_DATA, decode_op_data},
		{TFTP_OP_ACK, NULL},
		{TFTP_OP_ERROR, decode_op_error},
		{0, NULL}
};


static int _ack(tftp_t* t, unsigned short block_no);
static int _initial_connect(tftp_t* t);

tftp tftp_open(tftp_open_settings* settings)
{
	void* netif;
	int err;
	struct timeval tv;
	tftp_t* context = (tftp_t*) malloc(sizeof(tftp_t));
	memset(context, 0, sizeof(tftp_t));
	memcpy(&context->settings, settings, sizeof(tftp_open_settings));

   context->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (context->sockfd < 0)
    {
    	printf("failed to open socket");
    	goto error;
    }

    __tftp_parse_url(settings->url, context->ip, &context->port, context->filename);

    bzero(&context->serveraddr, sizeof(context->serveraddr));
    context->serveraddr.sin_family = AF_INET;
    context->serveraddr.sin_addr.s_addr = inet_addr(context->ip);
    context->serveraddr.sin_port = htons(context->port);
    context->client_addr_len = sizeof(context->serveraddr);

    memset(&tv, 0, sizeof(tv));
    tv.tv_sec = TFTP_RECV_TIMEOUT;
    setsockopt(context->sockfd, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&tv,sizeof(struct timeval));
    setsockopt(context->sockfd, SOL_SOCKET, SO_SNDTIMEO,(struct timeval *)&tv,sizeof(struct timeval));

    return (tftp) context;

   error:
	if(context != NULL)
	{
		free(context);
		context= NULL;
	}
	return NULL;
}

/**
 * we support only
 * - RRQ
 * - octet (binary)
 */
int tftp_request(tftp t)
{
	unsigned char payload[512 + 5] = {0};
	int nread;
	char* ptr;
	unsigned short opcode;
	unsigned short block_no;
	ioutil io;
	int rc = 0;

	tftp_t* context = (tftp_t*) t;
	rc = _initial_connect(t);
    for(;;)
    {
        memset(payload, 0, sizeof(payload));
        nread = recvfrom(context->sockfd, (void *)payload, sizeof(payload), 0, ( struct sockaddr *)&context->serveraddr,  (socklen_t *)&context->client_addr_len);
        ptr = payload;
        io = ioutil_new(&ptr, sizeof(payload));
        opcode = ioutil_read2(io);

        if(opcode == 0 || opcode > TFTP_OP_ERROR)
        {
        	printf("0 < opcode < 6, natural number");
        	ioutil_delete(io);
        	return -1;
        }

        if(s_op_func[opcode].fp != NULL)
        	s_op_func[opcode].fp(t, io);

        ioutil_delete(io);
        if(context->settings.callback != NULL) {
        	rc = context->settings.callback(context->settings.context, ((nread - 4) < TFTP_BLOCKSIZE) ? tftp_complete: tftp_downloading, &payload[4], nread - 4);
        	if(rc != 0) {
        		break;
        	}
        }

        if((nread - 4) < TFTP_BLOCKSIZE)
        	break;
    }
    return rc;
}

int tftp_close(tftp t)
{
	tftp_t* context = (tftp_t*) t;
	if(context != NULL)
	{
		close(context->sockfd);
		free(context);
	}
	return 0;
}


static int _ack(tftp_t* t, unsigned short block_no)
{
	unsigned char buffer[64] = {0};
	ioutil io;
	int len;
	int nwrite;

	char* ptr = buffer;
	io = ioutil_new(&ptr, sizeof(buffer));
	ioutil_write2(io, TFTP_OP_ACK);
	ioutil_write2(io, block_no);
	len = ioutil_get_offset(io);
	ioutil_delete(io);

	nwrite = sendto(t->sockfd, (void *)buffer, len, 0, (struct sockaddr *)&t->serveraddr,   t->client_addr_len);
	if(nwrite != len)
	{
		printf("nwrite != len");
	}
	return 0;
}

static int _initial_connect(tftp_t* t)
{
    unsigned char payload[1024] = {0};
    unsigned char* ptr;
    int size;
    ioutil io;
    int rc;

    memset(payload, 0, sizeof(payload));
    ptr = payload;
    io = ioutil_new((char**)&ptr, (int)sizeof(payload));
    ioutil_write2(io, (unsigned short) TFTP_OP_RRQ);
    ioutil_writen(io, strlen(t->filename) , t->filename);
    ioutil_write1(io, 0);
    ioutil_writen(io, strlen("octet\0") ,"octet\0");
    ioutil_write1(io, 0);

    size = ioutil_get_offset(io);
    ioutil_delete(io);
    ptr = NULL;
    H_(payload, size);
    rc = sendto(t->sockfd, (void *)payload, size, 0, (struct sockaddr *)&t->serveraddr, t->client_addr_len);
    return rc;
}

static int __tftp_parse_url(const char* url, _out_ char* ip, _out_ unsigned short* port, _out_ char* file)
{
	stringtokenizer tokenizer;
	char* ptr;
	int i, ret;
	char temp[512];

	ptr = (char*)url;
	memset((void*)temp, 0, sizeof(temp));
	tokenizer = stringtokenizer_new((const char**)&ptr);

	for(i = 0; (ret = stringtokenizier_next_token(tokenizer, ":/", 2, temp)) != -1; i++)
	{
		switch(i) {
		case 0:
			// It must be "tftp"
			break;

		case 3:
			memcpy(ip, temp, strlen(temp));
			break;

		case 4:
			*port = atoi(temp);
			break;

		case 5:
			memcpy(file, temp, strlen(temp));
			break;
		}
	}
	stringtokenizer_delete(tokenizer);
	return 0;
}



DECODE_FUNC(decode_op_data)
{
	int block_no = ioutil_read2(io);
	_ack((tftp_t*)t, block_no);
	return 0;
}

DECODE_FUNC(decode_op_error)
{
	int block_no = ioutil_read2(io); // this is error code
	char buffer[512];
	ioutil_read_null_terminated_string(io, buffer);
	printf("decode_op_error: %s\n", buffer);
	return 0;
}

