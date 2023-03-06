#include <stdio.h>
#include <stdint.h>
#include "./test/test.h"

int main()
{
    uint8_t error=0;
    error|=test1();
    error|=test2();
    error|=test3();
    print_result("\n\nTESTS",error);
    getchar();
    printf("\n");
    return error;
}
