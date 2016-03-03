/* itoa example */
#include <stdio.h>
#include <stdlib.h>

int main ()
{
	char test[4] = "123";
	printf("char : %s\n", test);
	
	int changedTest = atoi(test);
	printf("int : %d\n", changedTest);
	
	int compareInt = 123;
	if(compareInt == changedTest) {
		printf("Ture\n");
	}
	else {
		printf("False\n");
	}
	
	return 0;
}
