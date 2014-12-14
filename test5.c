#include "dynamic.c"

int main()
{
    printf("redundant freeing\n");
    void* p = malloc(10);
    free(p);
    free(p);
    return 0;
}
