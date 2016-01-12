/*
 * tftp.h
 *
 *  Created on: 2011. 10. 11.
 *      Author: buttonfly
 *      : This implementation supports only download.
 *
 */

#ifndef TFTP_H_
#define TFTP_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * type definition
 */
typedef enum
{
	tftp_get = 0,
	tftp_put
} tftp_opcode;

typedef enum
{
	tftp_downloading = 0,
	tftp_complete
} tftp_state;

typedef int (*tftp_callback)(void* context, tftp_state state, unsigned char* data, int len);

typedef struct
{
	tftp_callback callback;
	void* context;
	char* url;
} tftp_open_settings;
//Its size MUST be matched with <knDownloadOpenSettings>

typedef void* tftp;


/*
 * function prototypes
 */

tftp tftp_open(tftp_open_settings* settings);

int tftp_request(tftp t);

int tftp_close(tftp t);

#ifdef __cplusplus
}
#endif

#endif /* TFTP_H_ */
