/* itoa example */
#include <stdio.h>
#include <stdlib.h>
#include <string>

int main ()
{
	// char test[4] = "123";
	std::string test = "0123";
	printf("char : %s\n", test.c_str());
	
	printf("int : %d\n", atoi(test.c_str()));
	
	unsigned int compareInt = 123;
	if(compareInt == static_cast<unsigned int>(atoi(test.c_str()))) {
		printf("Ture\n");
	}
	else {
		printf("False\n");
	}
	
	return 0;
}
