/* itoa example */
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	if(argc != 2) {
		return -1;
	}
	printf("input value : %s\n", argv[1]);
	
	struct in_addr address;
	inet_aton(argv[1], &address);
	printf("hex : %u\n", address);
	printf("str : %s\n", inet_ntoa(address));
	
	return 0;
}
