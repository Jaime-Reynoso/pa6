#include "dynamic.c"

int main()
{


        printf("mallocing p 20000\n");
        void *p = malloc(20000);
        printf("p 20000 '%p'\n", p);
       
        printf("mallocing p2 1000\n");
        void *p2 = malloc(1000);
        printf("p2 1000 '%p'\n", p2);

        printf("reallocing p2 1500\n");
        p2=realloc(p2,1500);
        printf("p2 1500 '%p'\n", p2);

        printf("Freeing p\n");
        free(p);

        printf("mallocing p3 99\n");
        void *p3 = malloc(99);

        printf("p3 99 '%p'\n", p3);

        printf("Free p2\n");

        free(p2);

        printf("Free p3\n");

        free(p3);
        
        printf("done with simple test\n");

        return 0;
}