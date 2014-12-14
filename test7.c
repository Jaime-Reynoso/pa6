#include "dynamic.h"
#include <stdio.h>

void main()
{
	printf("for not returned from  malloc \n");
        p = (char *)malloc( 200 ); 
        free( p + 10 );

}