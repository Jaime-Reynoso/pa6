#include <stdio.h>
#include <string.h>

typedef long Align;

/* The align is there as a minimum to the amount of space you need to allocate */
union header{
	struct{
		union header *ptr;
		unsigned size;
		short int usedFlag;
	} s;
	Align x;
};

typedef union header Header;

void* myMalloc(unsigned nbytes, const char* file, const int line);
static Header* myMorecore(unsigned nu);
void myFree(void *ap, const char* file, const int line);
void *getMoreMem(int neededMemory);
void *myRealloc(void *ptr, size_t size, const char* file, const int line);
void* myCalloc(size_t num, size_t size, const char* file, const int line);
