#include "dynamic.h"
#include <stdio.h>

int main()
{
    int i;
    for(i = 0; i < 5; i++)
    {
        int* p = malloc(sizeof(int));
        *p = i;
        printf("address is %p, value is %d\n",p, *p);
    }
    return 0;
}