/*
 * osl_eventque.c
 *
 *  Created on: Mar 8, 2015
 *      Author: buttonfly
 */




#include "osl.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <poll.h>
#include <linkedlist.h>
#include "osl.h"
#include "osldbg.h"


// THIS IS UNSTABLE
#define UNIX_DOMAIN_SOCKET_SUPPORT 1

#if UNIX_DOMAIN_SOCKET_SUPPORT
static linkedlist _ques = NULL;

typedef struct {
	int fd;
	char* name;
	int ref;
	osl_mutex lck;
	fd_set readfds;
	struct sockaddr_un  serveraddr;
} osl_eventque_t;


static osl_eventque_t* find_eventque_by_name(const char* name);

osl_eventque osl_eventque_open(const char* name) {
	S_;
	osl_eventque_t* que = find_eventque_by_name(name);
	if(que) {
		que->ref++;
		return que;
	}

	que = (osl_eventque_t*) malloc(sizeof(osl_eventque_t));
	KASSERT(que);

	que->lck = osl_mutex_create();
	KASSERT(que->lck);
	que->name = strdup(name);

	char file[1024] = {0};
	sprintf(file, "/tmp/%s", name);
	if (access(file, R_OK | W_OK) == 0) {
		unlink(file); // It is weired. It must not exist.
	}

	que->fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (que->fd  < 0) {
		fprintf(stderr, "failed to create a socket descriptor");
		return NULL;
	}

	memset(&que->serveraddr, 0, sizeof(que->serveraddr));
	que->serveraddr.sun_family = AF_UNIX;
	strcpy(que->serveraddr.sun_path, file);

	bind(que->fd, (struct sockaddr*) &que->serveraddr, sizeof(que->serveraddr));
	listen(que->fd, 1);

	linkedlist_add(_ques, que);
	return que;
}

int osl_eventque_close(osl_eventque handle) {
	osl_eventque_t* que = (osl_eventque_t*) handle;
	if(que->ref > 0) {
		que->ref--;
		return 0;
	}

	if(que->lck) osl_mutex_close(que->lck);
	que->lck = NULL;

	char file[1024] = {0};
	sprintf(file, "/tmp/%s", que->name);
	unlink(file);
	close(que->fd);
	free(que);
	return 0;
}

int osl_eventque_get(osl_eventque handle, osl_event* event) {
	S_;
	int nread = 0;
	int offset;
	int clientsocket = -1;
	const int sizeof_event = sizeof(*event);
	struct sockaddr_un clientaddr;
	int clientaddr_len;
	clientaddr_len = sizeof(clientaddr);

	osl_eventque_t* que = (osl_eventque_t*) handle;
//	osl_mutex_lock(que->lck);
	clientsocket = accept(que->fd, (struct sockaddr_un*) &clientaddr, (socklen_t*) &clientaddr_len);
	if(clientsocket < 0) {
		fprintf(stderr, "invalid client socket: %d", clientsocket);
		goto error;
	}

	ioctl(clientsocket, FIONREAD, &nread);
	if(nread > 0) {
		m_("nread : %d", nread);
		osl_mutex_lock(que->lck);
#if 1
		for(nread=0, offset=0; offset < sizeof_event; offset+=nread) {
			nread = read(clientsocket, event + offset, sizeof_event - offset);
	//		usleep(10);
			fprintf(stderr, ".");
		}
#else
		nread = osl_read(clientsocket, event, sizeof_event);
#endif
		osl_mutex_unlock(que->lck);
	}
	close(clientsocket);
	return 0;

error:
	osl_mutex_unlock(que->lck);
	if(clientsocket>0) close(clientsocket);
	return -1;
}

int osl_eventque_put(osl_eventque handle,  osl_event* event) {
	S_;
	int fd;
	int ret;
	struct sockaddr_un serveraddr;
	int nwrite, offset;
	const int sizeof_event = sizeof(osl_event);

	osl_eventque_t* que = (osl_eventque_t*) handle;
	KASSERT(que);
//	if(!(event->event & event_queue->events)) {
//		fprintf(stderr, "event <%d> filtered", event->event);
//		return -1;
//	}

	char file[1024] = {0};
	sprintf(file, "/tmp/%s", que->name);

//	osl_mutex_lock(que->lck);
	fd = socket(PF_UNIX, SOCK_STREAM, 0);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, file);
	int clientaddr_len;
	clientaddr_len = sizeof(serveraddr);

    ret = connect(fd, (struct sockaddr *) &serveraddr, clientaddr_len);
	if(ret == -1) {
		fprintf(stderr, "oops! cannot connect to ");
		goto error;
	}

	osl_mutex_lock(que->lck);
	for(nwrite=0, offset=0; offset < sizeof_event; offset+=nwrite) {
		m_("%d, %x, %d", fd, event+offset, sizeof_event - offset);
		nwrite = write(fd, event + offset, sizeof_event - offset);
		KASSERT(nwrite > 0);
		fprintf(stderr, ",%d", nwrite);
	}
	osl_mutex_unlock(que->lck);
	close(fd);

//	usleep(10); // yield
	return 0;

error:
	osl_mutex_unlock(que->lck);
	close(fd);
	return -1;
}

__attribute__((constructor)) void begin (void) {
 /* Function Body */
	S_;
	_ques = linkedlist_new();
	KASSERT(_ques);
}
__attribute__((destructor)) void end (void)
{
	S_;
 /* Function Body */
	linkedlist_delete(_ques, NULL); // TODO
}

static osl_eventque_t* find_eventque_by_name(const char* name) {
	KASSERT(_ques);
	int i = 0;
	for(i = 0; i < linkedlist_size(_ques); i++) {
		osl_eventque_t* que = linkedlist_get(_ques, i);
		if(strcmp(que->name, name)==0) {
			return que;
		}
	}
	return NULL;
}

#else
#endif
