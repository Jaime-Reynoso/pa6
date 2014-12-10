#include <stdio.h>

typedef long Align;

union header{
	struct{
		union header *ptr;
		unsigned size;
	} s;
	Align x;
};

typedef union header Header;

void* myMalloc(unsigned nbytes);
static Header *myMorecore(unsigned nu);
void myfree(void *ap);