#include "dynamic.c"

int main()
{
    int* p = malloc(sizeof(int));
    *p = 5;
    printf("address is %p, value is %d\n",p, *p);
    free(p);
    return 0;
}
