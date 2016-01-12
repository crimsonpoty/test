/*
 * type.h
 *
 *  Created on: Sep 1, 2015
 *      Author: buttonfly
 */

#ifndef COREMEDIA_TYPE_H_
#define COREMEDIA_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define INVALID_PID 0x1FFF
#define INFINITE	0xFFFFFFFF
#define MPEG_DASH_MAX_SEGMENT_SIZE  (24*1024*1024)

typedef unsigned long NativeHandle;

#ifndef YES
#define YES true
#endif

#ifndef NO
#define NO false
#endif

#define OK		0
#define NOK	-1

#define DATASOURCE_UPDATED	1


#ifdef __cplusplus
}
#endif

#endif /* COREMEDIA_TYPE_H_ */
