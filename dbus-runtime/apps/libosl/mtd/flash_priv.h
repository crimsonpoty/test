/*
 * flash_priv.h
 *
 *  Created on: Jun 14, 2013
 *      Author: buttonfly
 */

#ifndef FLASH_PRIV_H_
#define FLASH_PRIV_H_

#ifdef __cpluscplus
extern "C"
{
#endif


#define PROGRAM_NAME "nut"

#include <stdint.h>
#include "libmtd.h"
#include "common.h"

typedef struct {
	int mtdno;
	int fd;
	char device[64];
	struct mtd_dev_info mtd;
	libmtd_t desc;
} osl_mtd_t;

#ifdef __cplusplus
}
#endif

#endif /* FLASH_PRIV_H_ */
