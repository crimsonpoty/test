#include <stdio.h>

enum ETEST
{
    eTEST_1,
    eTEST_2,
    eTEST_3,
    eTEST_4,
    eTEST_5,
    eTEST_MAX
};

int main() {
    
    ETEST eTest = eTEST_1;

    for(ETEST i = static_cast<ETEST>(0); i < eTEST_MAX; i++) {
        //eTest = static_cast<ETEST>(i);
        eTest = i;
        printf("eTest(%d)\n", eTest); 
    } 

	return 0;
}
