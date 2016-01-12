/*
 * nor_priv.c
 *
 *  Created on: Mar 29, 2012
 *      Author: buttonfly
 */


#include "flash_priv.h"
#include "osl.h"
#include "osldbg.h"
#include <mtd/mtd-user.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <mtd/mtd-user.h>
#include <mtd/jffs2-user.h>
#include <stdbool.h>
//#define USE_MTDLIB


int32_t ProgramNorFlash(osl_mtd handle, u_int32_t offset, u_int8_t* buf, u_int32_t length) {
	osl_mtd_t* context = (osl_mtd_t*)handle;
	int nwrite;
#ifdef USE_MTDLIB
	int rc;
	rc = mtd_write(
			context->desc,
			&context->mtd,
			context->fd,
			offset / context->mtd.eb_size,
			offset % context->mtd.eb_size,
			buf,
			context->mtd.min_io_size,
			NULL,
			0,
			MTD_OPS_RAW);
	if(rc) {
		fprintf(stderr, "failed to <mtd_write>\n");
		return -1;
	}
#else
	lseek(context->fd, offset, SEEK_SET);
	nwrite = write(context->fd, buf, length);
	if(nwrite != length) {
		W_("length: %d, nread: %d", length, nwrite);
	}
#endif
	return length;
}


int32_t ReadNorFlash(osl_mtd handle, u_int32_t offset, u_int8_t* buf, u_int32_t length) {
	int nread = 0;
	osl_mtd_t* context = (osl_mtd_t*)handle;
	lseek(context->fd, offset, SEEK_SET);
	nread = read(context->fd, buf, length);
	return nread;
}

