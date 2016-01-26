#include <cstdio>
#include <string.h>

using namespace std;

int main() {
    int a = 2;
    switch(a)
    {
    case 1:
        break;
    case 2:
    {
	    for(int i = 0; i < 10; i++) {
            if(i == 5) {
                break;
            }
            printf("%d\n", i);
	    }
        printf("is break?\n");
    }
    break;
    }

	return 0;
}

