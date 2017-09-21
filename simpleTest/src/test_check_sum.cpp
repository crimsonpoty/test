#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define NOR 1 * 1024 * 1024

int main(int argc, char *argv[])
{
// mmap
/*
		int fd;
	unsigned char *buf;
	struct stat st;

	stat(argv[1], &st);
	fd = open(argv[1], O_RDONLY);
	buf = (unsigned char *) mmap (0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	if(buf == MAP_FAILED) {
		printf("MAP_FAILED\n");
		return(1);
	}
	close (fd);

	int i;
	for(int i = 0; i < st.st_size; i++) {
		CheckSum += *pBuffer++;
	}	

	printf("size %04X\n", check_sum);
*/ 

	if(argc != 2) {
		printf("usage: %s \"input_file_name\"\n", argv[0]);
		return 0;
	}

// file
	int fd;
	struct stat st;
	
	stat(argv[1], &st);
	unsigned char *pBuffer = (unsigned char *)malloc(st.st_size);
	if(pBuffer == NULL) {
		printf("malloc error\n");
		return 0;
	}	
	
	if( 0 < ( fd = open(argv[1], O_RDONLY))) {
		if(read(fd, pBuffer, st.st_size) > 0) {
			printf("Read Start \n");
			close(fd);
		}
		else {
			printf("read Fail!\n");
		}
	}
	else{
		printf("File Open Fail\n");
		free(pBuffer);
		return 0;
	}	
	printf("Read END \n");
	
	/**< CheckSum 계산 */
	unsigned short CheckSum = 0;
	for(int i = 0; i < st.st_size; i++) {
		CheckSum += *pBuffer++;
	}	
	
	printf("size %04X\n", CheckSum);
}
 
