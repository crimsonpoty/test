#include <stdio.h>
#include <stdlib.h>

void example_function(void)
{
    printf("function pointer\n");
}

int main()
{
    void (*write_reg)(void);
    
    write_reg = example_function;
    write_reg();
    
    return 0;
}
