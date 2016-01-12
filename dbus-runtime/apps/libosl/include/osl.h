/*
 * kaonsys.h
 *
 *  Created on: Sep 10, 2010
 *      Author: buttonfly
 */

#ifndef OSL_H_
#define OSL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#define MULTICAST_SUPPORT 1


typedef enum {
	OSL_SUCCESS = 0,
	OSL_ERROR_OUT_OF_MEMORY,
	OSL_ERROR_INVALID_PARAM,
	OSL_NOT_SUPPORTED,
	OSL_NOT_AVAILABLE,
	OSL_ERROR_BUSY,
	OSL_EXTERNAL_ERROR,
	OSL_INVALID_STATES,
	OSL_TIMEOUT,
	OSL_RESET_REQUIRED,
	OSL_FAILTURE,
	OSL_MUCH_TIME_DIFFERENCE_ERROR,
	OSL_SMARTCARD_ERROR,
	OSL_SMARTCARD_ERROR_RX_PARITY,
	OSL_SMARTCARD_ERROR_TX_PARITY,
	OSL_SMARTCARD_ERROR_RX_TIMEOUT,
	OSL_SMARTCARD_ERROR_TX_TIMEOUR,
	OSL_SMARTCARD_ERROR_HW_FAILURE,
	OSL_SMARTCARD_RESET_TERMINAL /* requires a call to bsmartcard_reset() */
} OSL_Error;


typedef void* osl;
typedef void* osl_mutex;
typedef void* osl_spin;
typedef void* osl_thread;
typedef void* osl_semaphore;
typedef void* osl_queue;
typedef void* osl_mtd;
typedef void* osl_tty;

#define OSL_INFINITE		0xFFFF
#define OSL_TIMEOUT		20

#define _out_

#ifndef MAKELONG
#define MAKELONG(l, h) 	((unsigned int)( (((unsigned short)l) & 0xFFFF) | ((((unsigned short)h) & 0xFFFF) << 16) ))
#endif

/* smartcard session id */
#ifndef LOWORD
#define LOWORD(l)				((unsigned short)(l & 0xFFFF))
#endif
 /* device id */
#ifndef HIWORD
#define HIWORD(l)				((unsigned short)((l >> 16) & 0xFFFF))
#endif

void osl_exec_async(void (*__function)(void*), void* param);

u_int32_t osl_get_clock_ms (void);
/**
 *
 * @function oslCreateThread
 * @param	function		pointer function
 * @param 	param			argument to be aligned when the [fucntion] is called
 * @param	stack			the specific stack size. if you set 0, it would be set as default stack size
 * @param	name			@deprecated
 * @return						handle
 */
osl_thread osl_thread_create (void (*__function)(void*), void* param, u_int32_t stack, u_int8_t priority, int8_t* name);

/**
 * @function oslThreadDelay
 * @param	timeout			millisecond
 */
void osl_delay (u_int32_t timeout);

/**
 * @function	oslThreadSleep
 * @param	timeout			second
 */
void osl_sleep (u_int32_t timeout);

/**
 * @function	KnOSGetCurrentTask
 * @return						current thread
 */
osl_thread osl_thread_current(void);

/**
 * @function	KnOSCreateSemaphore
 * @return	semaphore handle
 */
osl_semaphore osl_semaphore_create(void);

/**
 * @function KnOSCreateCountSemaphore
 * @param 	value
 */
osl_semaphore osl_semaphore_create_count(u_int32_t count);

/**
 * @function KnOSInitCountSemaphore
 * @param	semaphore	semaphore handle
 * @param	value
 * @deprecated		It is used internally
 */
void osl_semaphore_init(osl_semaphore semaphore, u_int32_t count);

/**
 * @function	KnOSDeleteSemaphore
 * @param	semaphore
 * @description
 */
void osl_semaphore_close(osl_semaphore semaphore);

/**
 * @function	KnOSSemaphoreWait
 * @param	semaphore
 * @description
 * The running status if the current thread is changed to the status of NOT_RUNNABLE
 */
void osl_semaphore_wait(osl_semaphore semaphore);
/**
 * @function KnOSSemaphoreSignal
 * @param	semaphore
 * @description
 * wake up
 */
void osl_semaphore_signal(osl_semaphore semaphore);

/***
 * @function	KnOSSemaphoreWaitTimeout
 * @param	semaphore
 * @param	timeout				milli
 * @return
 */
bool osl_semaphore_timed_wait(osl_semaphore semaphore, u_int16_t timeout);

/**
 * @function	KnOSCreateMutex
 * @return	mutex handle
 */
osl_mutex osl_mutex_create(void);

/**
 * @function	KnOSMutexLock
 * @param	handle
 */
void osl_mutex_lock(osl_mutex mutex);

/**
 * @function	KnOSMutexUnlock
 * @param	handle
 */
void osl_mutex_unlock(osl_mutex mutex);

/**
 * @function	KnOSMutexUnlock
 * @param	handle
 */
void osl_mutex_close (osl_mutex mutex);




osl_spin osl_spin_create(void);

/**
 * @function	KnOSMutexLock
 * @param	handle
 */
void osl_spin_lock(osl_spin lck);

/**
 * @function	KnOSMutexUnlock
 * @param	handle
 */
void osl_spin_unlock(osl_spin lck);

/**
 * @function	KnOSMutexUnlock
 * @param	handle
 */
void osl_spin_close (osl_spin lck);





u_int32_t osl_get_gateway_addr(void);

u_int32_t osl_get_hwaddr(u_int8_t *addr);

typedef void* osl_random;

osl_random osl_random_open();

int osl_random_read(osl_random random, u_int8_t* buf, u_int32_t len);

void osl_random_close(osl_random random);

int osl_get_random(u_int8_t *buf, int32_t len);


u_int32_t osl_get_ipaddr(const char* ifname);

int osl_get_ip(const char* ifname, char* buf, size_t len);


/* @return APDU format */
u_int32_t osl_current_time(void);


#if TODO_
typedef struct {
	u_int32_t					event;
	void*	data;
} osl_event;
int osl_event_open(int* const fd);
int osl_event_close(const int fd);
int osl_event_get(const int fd, osl_event* const event);
int osl_event_put(const int _fd, osl_event* const event);
#else // TODO
typedef struct {
	u_int32_t					event;
	void*	data;
} osl_event;
typedef void* osl_eventque;
osl_eventque osl_eventque_open(const char* name);
int osl_eventque_close(osl_eventque handle);
int osl_eventque_get(osl_eventque handle, osl_event* event);
int osl_eventque_put(osl_eventque handle,  osl_event* event);
#endif

osl_mtd osl_mtd_open(int mtdno, u_int32_t opt);
const char* osl_mtd_name(osl_mtd h);
int osl_mtd_write(osl_mtd h, u_int32_t offset, int8_t* buff, size_t len);
int osl_mtd_read(osl_mtd h, u_int32_t offset, int8_t* buff, size_t len);
int osl_mtd_erase(osl_mtd h, u_int32_t start, size_t len);
int osl_mtd_close(osl_mtd h);
int osl_mtd_count(void);
int osl_mtd_search_by_name(const char* name);

bool osl_file_exist(const char* path);
off_t osl_file_size(const char* path);
blksize_t osl_block_size(const char* path);
blkcnt_t osl_block_count(const char* path);
ssize_t osl_write(int fd, char* buf, size_t size);
ssize_t osl_read(int fd, char* buf, size_t size);

int osl_http_content_type(const char* uri, char* content_type);
int osl_http_download(const char* uri, const char* dst);

int osl_get_rx(const char* alias);
int osl_get_tx(const char* alias);

int osl_get_system_meminfo(unsigned long* free, unsigned long* total);

int osl_get_system_diskinfo(const char* path, unsigned long* free, unsigned long* total);

pid_t osl_get_pid(const char* process);
int osl_kill(pid_t pid);
int osl_load_average(double load[3]);

osl_tty osl_tty_open(const char* dev, int boudrate);
int osl_tty_read(osl_tty h, unsigned  char* buf, int len);
int osl_tty_write(osl_tty h, unsigned const char* buf, int len);
int osl_tty_close(osl_tty h);


int osl_get_host(char* host, _out_ char* ip) ;
int osl_connect(char* ip, uint16_t port) ;
int osl_udpconnect(char* ip, short port) ;
int osl_send(int sck, char* buf, int len);
int osl_recv(int sck, char* buf, int len, int timeout);
int osl_disconnect(int sck);


#if MULTICAST_SUPPORT
typedef void *conn;
conn* osl_multicast_server_open(const char *alias, const char* ip, short port);
void osl_multicast_server_send(conn* conn, const char *data, int len);
void osl_multicast_server_close (conn* con);
#endif

#if 0
#include <stdlib.h>
#define KASSERT
#else
#include <stdlib.h>
#define KASSERT(x)	do{ if(!(x)){ERR("Assertion failed in line"); abort(); }; }while(0)
#endif


#define MAKELONG(l, h) 	((u_int32_t)( (((u_int16_t)l) & 0xFFFF) | ((((u_int16_t)h) & 0xFFFF) << 16) ))
#define LOWORD(l)				((u_int16_t)(l & 0xFFFF))
#define HIWORD(l)				((u_int16_t)((l >> 16) & 0xFFFF))



#ifdef __cplusplus
}
#endif


#endif /* OSL_H_ */
