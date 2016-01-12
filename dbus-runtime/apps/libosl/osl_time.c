/*
 * osl_time.c
 *
 *  Created on: May 8, 2013
 *      Author: buttonfly
 */




#include <sys/types.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

u_int32_t osl_current_time(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	struct tm* ptm = localtime(&tv.tv_sec);
//	M_("%d %d-%d %d:%d.%d\"", ptm->tm_year + 1900, ptm->tm_mon, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

	u_int8_t year = ptm->tm_year % 10;
	u_int32_t temp = ptm->tm_year / 10;
	u_int8_t yearOffset = (temp / 10) + (temp % 10);
	u_int8_t day = ptm->tm_mday;
	u_int8_t month = ptm->tm_mon + 1; // zero base index
	u_int8_t hour = ptm->tm_hour;
	u_int8_t min = ptm->tm_min;

	// LINUX
	// transform to ADPU TIME format
	u_int32_t time = 0;
	u_int8_t* ptr = (u_int8_t* )&time;
	ptr[0] = (yearOffset << 5)  & 0xe0;
	ptr[0] |= (day & 0x1f);

	ptr[1] = (month & 0x0f);
	ptr[1] |= (year << 4) & 0xf0;

	ptr[2] = (hour & 0x1f);
	ptr[3] = ((min) & 0x3f);
	return time;
}
