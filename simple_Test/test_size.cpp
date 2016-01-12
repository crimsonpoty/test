#include <cstdio>
#include <string.h>

using namespace std;

int main() {
	// char test = 'A';
	// char *ptest = &test;
	// char *paddr;
	// paddr = memcpy(ptest, &test, sizeof(char));
	// printf("ptest = %p\npaddr = %p\n", ptest, paddr);

	unsigned char zoneid[6];
	memset(zoneid, 0x00, sizeof(zoneid));
	
	for(int i = 0; i < 6; i++) {
		printf("zoneid[i]=0x%02X\n", zoneid[i]);
	}
	printf("size = %d\n", sizeof(zoneid));

	return 0;
}