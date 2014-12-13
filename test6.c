#include "dynamic.c"

int main()
{
    printf("pointers that were never allocated\n");
 	int x;
    free( &x );
    return 0;
}

