#include "dynamic.c"

int main()
{
	printf("for not returned from  malloc \n");
        char * p;
	p = (char *)malloc( 200 ); 
        free( p + 10 );
	return 0;
}
