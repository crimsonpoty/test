// #define printf(x, ...)

#include <stdio.h>
#include <stdlib.h>

// #define TTXOSD_DEBUG
// #define printf(x, ...)
// #define printf(...)

int main() {

	// char Name[]="Kim Sang Hyung";

	// printf("This is a String Constant.\n");
	// printf("This is a String Constant.%d\n", 1111);
	// TTXOSD_DEBUG("This is a String Constant. %d\n", 1);
	// puts(Name);

	// char ch[9] = {'f', 'a', 'c', 'i', 'l', 'i', 't', 'o', 'u'};
	
    /*
    static int count = 1;
	printf("A = %d\n", 'A');

	for(int i = 65; i < 99; i++, count++) {
		printf("i(%d), char(%c), count(%d),\n", i, (char)i, count);
	}
    */

    //unsigned char zoneId[3] = "15";
    //printf("zoneId(%s)\n", zoneId);
    
    char chip_id[12] = "00694967295";
    unsigned long int Num = atoi(chip_id);
    
    printf("%s, %0lu, %X\n", chip_id, Num, Num);

	return 0;
}
