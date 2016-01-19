#include <stdio.h>
#include <map>
#include <string>

int main() { 
	FILE *fp;
	fp = fopen("bca.bin", "rb");
	if(NULL == fp) {
		printf("Fail to fopen!!!\n");
		return 0;
	}
	
	static unsigned char bca[1024];
	int size = fread(bca, 1, 1024, fp);
	if(size < 0) {
		printf("Fail to fread!!! it is null file maybe\n");
	}
	
	fclose(fp);
	
	printf("PRINT: file size(%d)\n", size);

	return 0;
}
