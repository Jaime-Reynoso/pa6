#include "dynamic.h"
#include <stdio.h>

void main()
{
	printf("redundant freeing\n");
    void* p = malloc(10);
    free(p);
    free(p);
}