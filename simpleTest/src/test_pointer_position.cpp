#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

int main()
{
	bool aSub[9] = {0, 0, 1, 0, 0, 0, 0, 0, 0};
	// memset(aSub, true, sizeof(aSub));
	
	int y = std::find(aSub, aSub + (sizeof(aSub)/sizeof(bool)), true) - aSub;
	
	printf("%d\n", y);
	
    return 0;
}
