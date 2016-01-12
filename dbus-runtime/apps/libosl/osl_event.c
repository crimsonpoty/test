
/*
 * osl_event.c
 *
 *  Created on: Oct 9, 2012
 *      Author: buttonfly
 */

/**
 * @description:
 * We assume that a event-generator and a event-receiver runs on the same process.
 * However, we go with UNIX DOMAIN SOCKET because the client software use 'poll' API to detect an event.
 * UNIX_DOMAIN_SOCKET meets all needs in this case.
 */

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

#include "osl.h"

#if 1
#define UNIX_DOMAIN_FILE_PATH	"/tmp/.un"

struct osl_event_queue {
	int fd;
	uint32_t events;
	osl_mutex lck;
	fd_set readfds;
	struct sockaddr_un  serveraddr;
};

/* internal use functions */
static struct osl_event_queue* __get_instance();
static int __init(struct osl_event_queue* event_queue, const char* file);

int osl_event_open(int* const fd)
{
	struct osl_event_queue* event_queue =__get_instance();
	if(fd) {
		*fd = event_queue->fd;
	}
	return event_queue->fd;
}

int osl_event_close(const int fd)
{
	struct osl_event_queue* event_queue = __get_instance();
	unlink(UNIX_DOMAIN_FILE_PATH);
	close(event_queue->fd);
	free(event_queue);
	return 0;
}

int32_t osl_event_subscribe(const int fd, const uint32_t events)
{
	struct osl_event_queue* event_queue =__get_instance();
	event_queue->events = events;
	return 0;
}

int osl_event_get(const int fd, osl_event* const event)
{
#if 1
	int nread, offset;
	int clientsocket = -1;
	const int sizeof_event = sizeof(*event);
	struct sockaddr_un clientaddr;
	int clientaddr_len;
	clientaddr_len = sizeof(clientaddr);

	struct osl_event_queue* self =__get_instance();
	osl_mutex_lock(self->lck);
	clientsocket = accept(fd, (struct sockaddr_un*) &clientaddr, (socklen_t*) &clientaddr_len);

	if(clientsocket < 0) {
		fprintf(stderr, "invalid client socket: %d", clientsocket);
		goto error;
	}

	ioctl(clientsocket, FIONREAD, &nread);
	if(nread == 0) {
		fprintf(stderr, "nread: %d", nread);
		goto error;
	}

	for(nread=0, offset=0; offset < sizeof_event; offset+=nread) {
		nread = read(clientsocket, event + offset, sizeof_event - offset);
		usleep(10);
		fprintf(stderr, ".");
	}
	close(clientsocket);
	osl_mutex_unlock(self->lck);
	return 0;

error:
	close(clientsocket);
	osl_mutex_unlock(self->lck);
	return -1;
#else
	struct pollfd pollfds[1];
	pollfds[0].fd = fd;
	pollfds[0].events = POLLIN;
	pollfds[0].revents = 0;
	while(poll(pollfds, 1, -1) > 0) {
		if(pollfds[0].revents & POLLIN) {

		}
	}
//	int ret = MV_SUCCESS;
//
//	memset(event, 0, sizeof(*event));
//
//	struct pollfd pollfds[1];
//	pollfds[0].fd = k::eventque::get()->get_fd();
//	pollfds[0].events = POLLIN;
//	pollfds[0].revents = 0;
//
//	while(poll(pollfds, 1, -1) > 0) {
//		if(pollfds[0].revents & POLLIN)
//		{
//			ret = k::eventque::get()->get_event(event);
//			m_("event type: %d", event->event_type);
//			m_("event flag: %d", event->flags);
//			m_("event keycode: 0x%x", event->message.key.key_code);
//			m_("event mode: %d", event->message.key.key_mod);
//			m_("event subtype: %d", event->message.key.subtype);
//			m_("event time %u", event->time);
//			break;
//		}
//	}
#endif
}

//int osl_event_put(const int fd, osl_event* const event) {
//	return oslEventQuePut(fd, event);
//}

int osl_event_put(const int _fd, osl_event* const event)
{
	int fd;
	int ret;
	struct sockaddr_un serveraddr;
	int nwrite, offset;
	const int sizeof_event = sizeof(*event);

	struct osl_event_queue* event_queue =__get_instance();
	if(!(event->event & event_queue->events)) {
		fprintf(stderr, "event <%d> filtered", event->event);
		return -1;
	}

	osl_mutex_lock(event_queue->lck);
	fd = socket(PF_UNIX, SOCK_STREAM, 0);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sun_family = AF_UNIX;
    strcpy(serveraddr.sun_path, UNIX_DOMAIN_FILE_PATH);

    ret = connect(fd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
	if(ret == -1) {
		fprintf(stderr, "oops! cannot connect to ");
		goto error;
	}

	for(nwrite=0, offset=0; offset < sizeof_event; offset+=nwrite) {
		nwrite = write(fd, event + offset, sizeof_event - offset);
		usleep(10);
		fprintf(stderr, ",");
	}

	close(fd);
	osl_mutex_unlock(event_queue->lck);
	//usleep(100); // yield
	return 0;

error:
	close(fd);
	osl_mutex_unlock(event_queue->lck);
	return -1;


//	int fd;
//	int ret;
//	struct sockaddr_un serveraddr;
//	int nwrite, offset;
//	const int sizeof_event = sizeof(MvEvent);
//	m_("size of event: %d", sizeof_event);
//
//	osl_mutex_lock(_lck);
//	fd = socket(PF_UNIX, SOCK_STREAM, 0);
//    bzero(&serveraddr, sizeof(serveraddr));
//    serveraddr.sun_family = AF_UNIX;
//    strcpy(serveraddr.sun_path, _file.c_str());
//
//    ret = connect(fd, (struct sockaddr *) &serveraddr, sizeof(serveraddr));
//	if(ret == -1) {
//		fprintf(stderr, "oops! cannot connect to ");
//		osl_mutex_unlock(_lck);
//		close(fd);
//		return -1;
//	}
//
//	for(nwrite=0, offset=0; offset < sizeof_event; offset+=nwrite) {
//		nwrite = write(fd, (char*)e + offset, sizeof_event - offset);
//		usleep(10);
//	}
//
//	close(fd);
//	osl_mutex_unlock(_lck);
//	usleep(100); // yield
//	return 0;
}

static struct osl_event_queue* __get_instance()
{
	static struct osl_event_queue* event_queue = NULL;
	if(event_queue == NULL) {
		event_queue = malloc(sizeof(*event_queue));
		memset(event_queue, 0, sizeof(*event_queue));
		event_queue->lck = osl_mutex_create();
		__init(event_queue, UNIX_DOMAIN_FILE_PATH);
	}
	return event_queue;
}

static int __init(struct osl_event_queue* self, const char* file)
{
	if (access(file, R_OK | W_OK) == 0) {
		unlink(file);
	}

	self->fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if (self->fd  < 0) {
		fprintf(stderr, "failed to create a socket descriptor");
		return -1;
	}

	memset(&self->serveraddr, 0, sizeof(self->serveraddr));
	self->serveraddr.sun_family = AF_UNIX;
	strcpy(self->serveraddr.sun_path, file);

	bind(self->fd, (struct sockaddr*) &self->serveraddr, sizeof(self->serveraddr));
	listen(self->fd, 5);
	return 0;
}

#else

typedef struct {
	int fd;
	uint32_t events;
	osl_mutex lck;
	fd_set readfds;
	struct sockaddr_un  serveraddr;
} osl_event_t;

osl_event osl_event_open(const char* name) {
	char path[1024];
	sprintf(path, "/tmp/%s", name);

}

int osl_event_close(osl_event handle) {

}

int osl_event_get(osl_event handle, osl_ev* const event) {

}

int osl_event_put(osl_event handle,  osl_ev* const event) {

}

__attribute__((constructor)) void begin (void) {
 /* Function Body */
}
__attribute__((destructor)) void end (void)
{
 /* Function Body */
}

#endif


