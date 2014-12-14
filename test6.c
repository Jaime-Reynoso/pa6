#include "dynamic.h"
#include <stdio.h>

void main()
{
	
    printf("pointers that were never allocated\n");
 	int x;
    free( &x );
}

