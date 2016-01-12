/*
 * osl_tty.c
 *
 *  Created on: Apr 8, 2014
 *      Author: buttonfly
 */


#include "osl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "osldbg.h"

typedef struct {
	int fd;
} osl_tty_t;

osl_tty osl_tty_open(const char* dev, int boudrate) {

	int error;
	osl_tty_t* context = (osl_tty_t*) malloc(sizeof(osl_tty_t));
	memset(context, 0, sizeof(osl_tty_t));

#if 1
	context->fd = open(dev, O_RDWR | O_NOCTTY | O_SYNC | O_NONBLOCK);
#else
	context->fd = open(dev, O_RDWR | O_NOCTTY | O_SYNC);
#endif
	KASSERT(context->fd>0);

	struct termios termio;
	memset(&termio, 0, sizeof(struct termios));
	error = tcgetattr(context->fd, &termio);
	KASSERT(error == 0);
	error = cfsetospeed(&termio, B460800);
	KASSERT(error == 0);
	error = cfsetispeed(&termio, B460800);
	KASSERT(error == 0);

#if fixme
	termio.c_cflag = boudrate | CRTSCTS | CS8 | CLOCAL | CREAD;
#else
//	termio.c_cflag = B460800 | CRTSCTS | CS8 | CLOCAL | CREAD;
	termio.c_cflag = (termio.c_cflag & ~CSIZE) | CS8;
#endif
	termio.c_iflag &= ~IGNBRK;
	termio.c_oflag = 0;
	termio.c_lflag = 0;
	termio.c_cc[VTIME] = 0;
	termio.c_cc[VMIN] = 0;
	termio.c_iflag &= ~(IXON | IXOFF | IXANY);
	termio.c_cflag |= (CLOCAL | CREAD);
	termio.c_cflag &= ~(PARENB | PARODD);
	termio.c_cflag &= ~CSTOPB;
	termio.c_cflag &= ~CRTSCTS;
#if 0
#define HEADER_LEN		4
//	unsigned char version[] = {0xAC, 0xC0, 0x01, 0x10, 0x00};
	unsigned char version[] = {0xAC, 0xC0, 0x01, 0x10, 0x00};
	unsigned char paring[] = {0xAC, 0xC0, 0x02, 0x11, 0x1E, 0x00};

	int chksum = HEADER_LEN + version[2] - 1;

	m_("chksum index: %d", chksum);
	int i;
	for(i = 0; i < HEADER_LEN + version[2] - 1; i++) {
		version[chksum] ^= version[i];
//		m_("%x - %x", version[chksum], version[i]);
	}
	int n = write(context->fd, version, sizeof(version));
	m_("write %d", n);
#endif

	tcflush(context->fd, TCIFLUSH);
	error = tcsetattr(context->fd, TCSANOW, &termio);
	KASSERT(error == 0);

	return (osl_tty) context;
}

int osl_tty_read(osl_tty h, unsigned char* buf, int len) {

	osl_tty_t* context = (osl_tty_t*) h;
	int n;
	int offset;
	for(n = 0, offset = 0; offset < len; offset += n) {
		n = read(context->fd, buf + offset, len - offset);
		if(n > 0) {
			H_((char*)(buf+offset), n);
		}
	}
	return offset;
}

int osl_tty_write(osl_tty h, const unsigned char* buf, int len) {
	osl_tty_t* context = (osl_tty_t*) h;
	int n;
	int offset;
	for(n = 0, offset = 0; offset < len; offset += n) {
		n = write(context->fd, buf + offset, len - offset);
		H_((char*)(buf+offset), n);
	}
	return offset;
}

int osl_tty_close(osl_tty h) {
	osl_tty_t* context = (osl_tty_t*) h;
	close(context->fd);
	free(context);
	return 0;
}


