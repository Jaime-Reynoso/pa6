#include "dynamic.c"

int main(){

	int *p = calloc(7, sizeof(int));
	int n;
	printf("Now we're testing whether calloc zero initialized everything \n");
	for(n = 0; n < 7; n++){
		printf("This is it: %d \n", p[n]);
	}
	return 0;
}