// #define printf(x, ...)

#include <stdio.h>
#include <stdlib.h>
#include <string>

// #define TTXOSD_DEBUG
// #define printf(x, ...)
// #define printf(...)

int main(int argc, char* argv[]) {

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
    
	// 한글 출력
	std::string Text("한글 출력");
	printf("%s\n", Text.c_str());
	Text.append(" append");
	printf("%s\n", Text.c_str());

	int test = 0;
	
	for(; test < 6; test++) {
		if(test == atoi(argv[1])) break;
	}
	printf("for test: %d\n", test);
	
	// 스페인어 출력
	unsigned char spanChar[2] = {0xC3, 0x91};
	printf("%s\n", std::string((const char *)spanChar).c_str());

	return 0;
}
