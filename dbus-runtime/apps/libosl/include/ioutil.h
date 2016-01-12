#ifndef IOUTIL_H_
#define IOUTIL_H_

/* ---------------------------------------------
 * ioutil.h
 * @author: smkim, buttonfly@gmail.com *
 * release date: 2008.4.14
 * --------------------------------------------*/


#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	char**    buffer;
	int        offset;
	int		total_length;

} _ioutil_t;

typedef void*    ioutil;

#if 0
//extern void ioutil_new_stack(char** buffer, _ioutil_t* io);
#endif

extern void ioutil_init(char** buffer, _ioutil_t* io, int total_length);

extern unsigned char ioutil_getbits8(unsigned char* x, int s, int n);

extern unsigned short ioutil_getbits16(unsigned char* x, int s, int n);

extern unsigned int ioutil_getbits32(unsigned char* x, int s, int n);


/**
 * get the crc32 value with the specified buffer
 * @param buffer byte array to be checksum
 * @return crc32 value
 */
extern unsigned long ioutil_get_crc32(char* buffer, int size);

/**
 * create the ioutil with the specified buffer.
 * make sure that the pointer of the specified buffer will be moved.
 * you need to have a variable which is assigned with the pointer of the specified buffer.
 * @return this ioutil
 */
extern ioutil ioutil_new(char** buffer, int total_length);

/**
 * read 4 bytes data from this ioutil's bytes.
 * we consider only in the case: little endian
 * @param ioutil this ioutil
 * @return 4 bytes value
 */
extern unsigned int ioutil_read4(ioutil ioutil);


/**
 * read 2 bytes data from this ioutil's bytes.
 * @return 2 bytes value
 */
extern unsigned short ioutil_read2(ioutil ioutil);

/**
 * read 1 byte data from this ioutil's byte
 * @param ioutil this ioutil
 * @return 1 byte value
 */
extern unsigned char ioutil_read1(ioutil ioutil);

/**
 * read n bytes data from this ioutil's bytes
 * @param ioutil this ioutil
 * @param size the size to read
 * @param dst an empty buffer which stores with the read data
 */
extern void ioutil_readn(ioutil ioutil, int size, void* dst);

extern void ioutil_read_skip(ioutil ioutil, int size);

extern unsigned  char ioutil_read_bits(ioutil ioutil, int s, int n);

extern unsigned short  ioutil_read2_bits(ioutil ioutil, int s, int n);

extern unsigned int ioutil_read3_bits(ioutil ioutil, int s, int n);

extern unsigned int ioutil_read4_bits(ioutil ioutil, int s, int n);

extern unsigned char ioutil_peek1(ioutil ioutil);

extern unsigned short ioutil_peek2(ioutil ioutil);

extern unsigned int ioutil_peek4(ioutil ioutil);

#if 0
extern void ioutil peekn(ioutil ioutil, int size, void* dst);
#endif

extern unsigned char ioutil_peek_bits(ioutil ioutil, int s, int n);


/**
 * write n bytes data to this ioutil's buffer
 * @param ioutil this ioutil
 * @param size the size to write
 * @param src the data to be store in the ioutil's buffer
 */
extern void ioutil_writen(ioutil ioutil, int size, void* src);

/**
 * write 4 bytes data to this ioutil's buffer
 * @param ioutil this ioutil
 * @param src the 4 bytes data to be stored
 */
extern void ioutil_write4(ioutil ioutil, int src);

/**
 * write 2 bytes data to this ioutil's buffer
 * @param ioutil this ioutil
 * @param src the 2 bytes data to be stored
 */
extern void ioutil_write2(ioutil ioutil, short src);

/**
 * write 1 byte data to this ioutil's buffer
 * @param ioutil this ioutil
 * @param src the 1 byte data to be stored
 */
extern void ioutil_write1(ioutil ioutil, char src);

/**
 * returns the current position which indicates the offset of the ioutil's buffer
 * @param ioutil this ioutil
 * @return the current position which indicates the offset of the ioutil's buffer
 */
extern int ioutil_get_offset(ioutil ioutil);

extern void ioutil_set_offset(ioutil ioutil, int offset);

extern int ioutil_get_total_length(ioutil ioutil);

extern unsigned char* ioutil_get_current_ptr(ioutil ioutil);

/**
 * close this ioutil
 * @param ioutil this ioutil
 */
extern void ioutil_delete(ioutil ioutil);

/**
 * returns the file size of the specified file descriptor
 * @param fd the file descriptor
 * @return the file size of the specified file descriptor
 */
extern int ioutil_get_file_size(int fd);


/**
 * read and allocate in the heap space from the specified path.
 * @param path full file path to read from
 * @param size buffer to be stored the file size
 */
extern void* ioutil_read_file(const char* path, int* size);

/**
 * split the file name from the full file path
 * @param path full file path to be splitted
 * @param filename buffer to be stored
 * @return
 */
extern int ioutil_split_filename(const char* path, char* filename);

extern void ioutil_write_file(const char* path, char* data, int len);


typedef void (* ioutil_file_detected)(void* context, const char* path);
void ioutil_find_file(const char* path, ioutil_file_detected cb, void* context);

int ioutil_get_file_extension(const char* path, char* ext);

unsigned int ioutil_read_null_terminated_string(ioutil ioutil, char* dst);

#ifdef __cplusplus
}
#endif

#endif /*IOUTIL_H_*/


