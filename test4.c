#include "dynamic.c"

int main()
{
	int *p;
	p = malloc(sizeof(int*));
	free(p + 100);
}
