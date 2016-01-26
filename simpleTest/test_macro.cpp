#include <cstdio>

#define TTXOSD_DEBUG printf
// #define TTXOSD_DEBUG(x)

int main(int argc, char *argv[]) {
	
	char Name[]="Kim Sang Hyung";

	// printf("This is a String Constant.\n");
	TTXOSD_DEBUG("This is a String Constant. %d\n", 1);
	puts(Name);

	return 0;
}