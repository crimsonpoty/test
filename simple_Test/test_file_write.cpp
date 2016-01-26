#include <stdio.h>
#include <map>
#include <string>

int main() { 
	FILE *fp;
	fp = fopen("test.bin", "wb");
	if(NULL == fp) {
		printf("Fail to fopen!!!\n");
		return 0;
	}
	
	static unsigned char bca[4] = {0x12, 0x23, 0x34, 0x45};
	int size = fwrite(bca, 1, 4, fp);
	if(size < 0) {
		printf("Fail to fwrite!!!\n");
	}
	
	fclose(fp);
	
	printf("PRINT: file size(%d)\n", size);

	return 0;
}
