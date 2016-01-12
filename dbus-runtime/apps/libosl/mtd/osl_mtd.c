/*
 * osl_mtd.c
 *
 *  Created on: May 8, 2013
 *      Author: buttonfly
 */


#include "osl.h"
#include "norflash_priv.h"
#include "nandflash_priv.h"
#include <sys/types.h>
#include <mtd/mtd-user.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "flash_priv.h"
#include <mtd/mtd-user.h>
#include <mtd/jffs2-user.h>
#include <stdbool.h>
#include <assert.h>


static libmtd_t get_mtd_handle();

osl_mtd osl_mtd_open(int mtdno, u_int32_t opt) {
	osl_mtd_t* context = (osl_mtd_t*) malloc(sizeof(osl_mtd_t));
	assert(context);
	context->mtdno = mtdno;
	int rc;

	context->desc = get_mtd_handle();

	sprintf(context->device, "/dev/mtd%d", (int)mtdno);

	context->fd = open(context->device, opt);
	if(context->fd < 0) {
		fprintf(stderr, "failed open %s", context->device);
		goto error;
	}

	rc = mtd_get_dev_info(get_mtd_handle(), context->device, &context->mtd);
	if(rc < 0) {
		fprintf(stderr, "failed to <mtd_get_dev_info>");
		goto error;
	}

	return (osl_mtd) context;

error:
	if(context != NULL) {
		free(context);
		context = NULL;
	}
	return NULL;
}

const char* osl_mtd_name(osl_mtd h) {
	osl_mtd_t* context = (osl_mtd_t*) h;
	return context->mtd.name;
}

int osl_mtd_write(osl_mtd h, u_int32_t offset, int8_t* buff, size_t len) {

	osl_mtd_t* context = (osl_mtd_t*) h;
	int rc;

	switch(context->mtd.type) {
	case MTD_NORFLASH:
		rc = ProgramNorFlash(h, offset, buff, len);
		break;

	case MTD_NANDFLASH:
		rc = ProgramNandFlash(h, offset, buff, len);
		break;

	default:
		assert(1);
		fprintf(stderr, "Not supported flash type\n");
		rc = -1;
		break;
	}
	return rc;
}

int osl_mtd_read(osl_mtd h, u_int32_t offset, int8_t* buff, size_t len) {
	osl_mtd_t* context = (osl_mtd_t*) h;
	int rc;

	switch(context->mtd.type) {
	case MTD_NORFLASH:
		rc = ReadNorFlash(h, offset, buff, len);
		break;

	case MTD_NANDFLASH:
		rc =ReadNandFlash(h, offset, buff, len);
		break;

	default:
		assert(1);
		fprintf(stderr, "Not supported flash type\n");
		rc = -1;
		break;
	}
	return rc;
}

int osl_mtd_erase(osl_mtd h, u_int32_t start, size_t len) {
	osl_mtd_t* context = (osl_mtd_t*) h;
	int rc;
	int eb_start = 0, eb_cnt = 0, eb = 0;
	bool isNand = false;
	uint64_t offset = 0;
	bool skipbad = false;
#if unused
	struct jffs2_unknown_node cleanmarker;
	int clmpos = 0, clmlen = 8;

	int error = 0;
#endif

	switch(context->mtd.type) {
	case MTD_NORFLASH:
		break;
	case MTD_NANDFLASH:
		isNand = true;
		break;
	default:
		assert(1);
		fprintf(stderr, "Not supported flash type\n");
		return -1;
	}

	eb_start = start / context->mtd.eb_size;

	if(len == 0) {
		eb_cnt = 0;
	}
	else {
		eb_cnt = (len / context->mtd.eb_size) - eb_start;
	}

	if (eb_cnt == 0)
		eb_cnt = (context->mtd.size / context->mtd.eb_size) - eb_start;

	for (eb = eb_start; eb < eb_start + eb_cnt; eb++) {
		offset = (uint64_t)eb * context->mtd.eb_size;

		if (skipbad == false) {
			int ret = mtd_is_bad(&context->mtd, context->fd, eb);
			if (ret > 0) {
				fprintf(stderr, "W: Skipping bad block at %08", offset);
				continue;
			}
			else if (ret < 0) {
				if (errno == EOPNOTSUPP) {
					skipbad = true;
					if (isNand) {
						fprintf(stderr, "Bad block check not available: %s",  context->device);
						assert(1);
						return -1;
					}
				}
				else {
					fprintf(stderr, "MTD get bad block failed, %s", context->device);
					assert(1);
					return -1;
				}
			}
		}

		rc = mtd_erase(get_mtd_handle(), &context->mtd, context->fd, eb);
		if(rc != 0) {
			fprintf(stderr, "failed to <mtd_erase>: %s", context->device);
			continue;
		}
		// JFFS2 Not supported, cleanmarker thing.
	}
	return rc;
}

int osl_mtd_close(osl_mtd h) {
	osl_mtd_t* context = (osl_mtd_t*)h;
	if(context != NULL) {
		close(context->fd);
		free(context);
		context = NULL;
	}
	return 0;
}

int osl_mtd_count(void) {
	libmtd_t desc = get_mtd_handle();
	struct mtd_info info;
	int error;
	error = mtd_get_info(desc, &info);
	return info.mtd_dev_cnt;
}

int osl_mtd_search_by_name(const char* name) {

	libmtd_t desc = get_mtd_handle();
	int count = osl_mtd_count();
	int i;
	for(i = 0; i < count; i++) {
		osl_mtd handle = osl_mtd_open(i, O_RDONLY | O_SYNC);
		if(handle==NULL) {
			return -1;
		}
		const char* _name = osl_mtd_name(handle);
		if(strcmp(name, _name) == 0) {
			osl_mtd_close(handle);
			break;
		}
		osl_mtd_close(handle);
	}
	return i;
error :
	return -1;
}

static libmtd_t get_mtd_handle()
{
	static libmtd_t mtd = NULL;
	if(mtd == NULL) {
		mtd =  libmtd_open();
	}
	return mtd;
}
