#include <stdio.h>
#include "defines.h"

extern void foobar2(void);
extern void foobar3(void);
extern void foobar4(void);

int main(int argc, char* argv[])
{
	printf("foobar, define value=%d\n", DEFINE_MY_VALUE);
	foobar2();
	foobar3();
	foobar4();
	return 0;
}
