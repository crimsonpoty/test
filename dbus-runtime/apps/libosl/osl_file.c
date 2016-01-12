/*
 * osl_file.c
 *
 *  Created on: Jul 22, 2013
 *      Author: buttonfly
 */


#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stddef.h>

bool osl_file_exist(const char* path) {
	int rc = access(path, F_OK);
	return (rc < 0) ? false : true;
}

off_t osl_file_size(const char* path) {
#if 0
	struct stat st;
	stat (path, &st);
	return st.st_size;
#else
	off_t size = 0;
	int fd = open(path, O_RDONLY | O_LARGEFILE);
	if(fd < 0) {
		return -1;
	}
	size = lseek(fd, 0, SEEK_END) + 1;
	close(fd);
	return size;
#endif
}

blksize_t osl_block_size(const char* path) {
	struct stat st;
	stat (path, &st);
	return st.st_blksize;
}

blkcnt_t osl_block_count(const char* path) {
	struct stat st;
	stat (path, &st);
	return st.st_blocks;
}


ssize_t osl_write(int fd, char* buf, size_t len) {

	ssize_t offset = 0;
	int n;
	for(	n =0; offset < len; offset += n) {
		n = write(fd, buf + offset, len - offset);
	}
	return(offset + 1);
}

ssize_t osl_read(int fd, char* buf, size_t len) {

	ssize_t offset = 0;
	int n = 0;
	for(	n =0; offset < len; offset += n) {
		n = read(fd, buf + offset, len - offset);
	}
	return(offset + 1);
}
