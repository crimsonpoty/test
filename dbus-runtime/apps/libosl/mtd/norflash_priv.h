/*
 * nor_priv.h
 *
 *  Created on: Mar 29, 2012
 *      Author: buttonfly
 */

#ifndef NORFLASH_PRIV_H_
#define NORFLASH_PRIV_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

int32_t ProgramNorFlash(osl_mtd handle, u_int32_t offset, u_int8_t* buf, u_int32_t length);

int32_t ReadNorFlash(osl_mtd handle, u_int32_t offset, u_int8_t* buf, u_int32_t length);

#ifdef __cplusplus
}
#endif

#endif /* NORFLASH_PRIV_H_ */
