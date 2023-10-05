#include<stdio.h>
#include<stdint.h>
int main()
{
    const static uint32_t x = 00000000000000000000000000000001;
    const static uint32_t y = 00000000000000000000000000000010;
    if ( x == y)
    {
        printf("hi\n");
    }
    else{
        printf("nope\n\n");
    }
}