#include "dynamic.c"

int main()
{
	int *p, i;
	printf("Here we're testing the Realloc function. We're allocating p, and inserting 7 int = 3\n");
	p = malloc(sizeof(int) * 7);
	
	for(i = 0; i < 7; i++){
		p[i] = 3;
	} 
	
	for(i = 0; i < 7; i++){
		printf("The value at %d is %d \n", i, p[i]);
	}

	printf("The original address of p is %p\n", p);
	
	p = realloc(p, sizeof(int )*9);
	printf("Now, we realloc p to something larger and the integers should still be in there\n");
	for(i = 0; i < 7; i++){
		printf("The value at %d is %d \n", i, p[i]);
	} 
	printf("The changed address is %p\n", p);
	printf("Here we are calling realloc to make the pointer smaller, but it should still be the same\n");
	p = realloc(p, sizeof(int)*3);
	printf("The address is %p\n", p);

	return 0;
}