#include <stdio.h>

int main() {
    
    unsigned int value = 2366791440;

    int checksum = (((value / 100) % 23) + (value % 100)) % 100;

    printf("checksum = %d\n", checksum);

	return 0;
}
