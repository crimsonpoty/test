/*
 * nand_priv.h
 *
 *  Created on: Mar 29, 2012
 *      Author: buttonfly
 */

#ifndef NANDFLASH_PRIV_H_
#define NANDFLASH_PRIV_H_

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

int32_t ProgramNandFlash(osl_mtd handle, u_int32_t offset, u_int8_t* buf, u_int32_t length);

int32_t ReadNandFlash(osl_mtd handle, u_int32_t offset, u_int8_t* buf, u_int32_t length);


#ifdef __cplusplus
}
#endif

#endif /* NANDFLASH_PRIV_H_ */
