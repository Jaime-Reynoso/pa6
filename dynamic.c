#include "dynamic.h"

#define maxMemory 4999;
#define usedFlagValue = 8732;
/*
*	predefined macros, these macros make it possible to tell where the error was
*/
#define malloc(x) myMalloc(x, __FILE__, __LINE__);
#define free(x) myFree(x, __FILE__, __LINE__);
#define realloc(ptr, x) myRealloc(ptr, x, __FILE__, __LINE__);
#define calloc(y, x) myCalloc(y, x, __FILE__, __LINE__);

static Header base[2]; /* This is the empty list */
static Header *smallfreep = NULL;	/*this is where the free list will begin for small free blocks */
static Header *largefreep = NULL;	/*this is where the free list will begin for large free blocks */
char virtualMemory[5000] = NULL;
static short int smallbreakpoint = 0;
static short int largebreakpoint = 5000;

/*
*	myMalloc is supposed to return a pointer to a free space in memory, so that segmentation faults become impossible. 
*/

void *myMalloc(unsigned nBytes, const char file, const int line){

	Header *p, *prevp;
	Header *myMorecore(unsigned);
	unsigned nunits;
	Header **freep;
	freep = &smallfreep;
	short int difference = 0;

	/* nunits is going to be the size of the block that we need */
	nunits = (nBytes+sizeof(Header) - 1)/sizeof(Header) + 1;
	/* since we need the block to be a multiple of Header we divide the nBytes by header size
	the + 1 is for the size*/

	/*In order to avoid fragmentation, I maintained 2 pointers; one called smallfreep and one called largefreep, largefreep will
	* point to anything larger than 100 bytes and freep will point to whichever one is appropriate */
	if (nunits > 100){
		freep = &largefreep;
		difference = 1;
	}

	/* This is the case in which the free linked list is empty, here the size is naturally 0 and the ptr points to itself*/
	if((prevp = *freep) == NULL){
		base[difference].s.ptr = *freep = prevp = &base;
		base[difference].s.size = 0;
		base[difference].s.usedFlag = usedFlagValue;
	}
	/* Now we need to traverse the free list and retrieve a formidable size*/

	for( p = prevp->s.ptr; ;prevp = p, p = p->s.ptr){ /*prevp keeps track of the last struct */
	/* This if statement asks if the pointer points to a space big enough to hold the data struct */
		if(p->s.size >= nunits){
			/* If the size of the pointer is just enough */
			if(p->s.size == nunits){
				/* then unlink p */
				prevp->s.ptr = p->s.ptr;
			}
			else{ /* else if the pointer space is larger than what we need */
				p->s.size -= nunits; /* decrease the size by the amount we need, leaving what we don't need*/
				p += p->s.size;	/* move the pointer to the beginning of the amount of space we need */
				p->s.size = nunits;	/* now the pointer points to a space that's just large enough for what we need */
			}

			*freep = prevp; /*freep points to the beginning of the free linked list structure */
			return (void*) (p+1); /* since we use 1 for the size, we return a pointer to where the structure will be hold */
		}
		if ( p == *freep) /* This is the case in which we cannot find a space large enough */
			if((p = myMorecore(nunits)) == NULL) /* myMoreCore will find more space, but if it can't then we return NULL */
			{
				fprintf(stderr, "Error: There's not enough memory to allocate line %d in file %s \n", line, file);
				return NULL;
			}
	}
}

/*
*	myMorecore is supposed to ask the system for more memory
*/
static Header * myMorecore(unsigned nu){

	char *cp, *getMoreMem(int);
	Header *up;
	Header **freep;
	freep = &smallfreep;

	/* NALLOC is the minimum amount of space that we want to look for
	* we want to minimize the amount of systemCalls we make because they're intensive on the runTime, so asking for more 
	* makes us ask less times */

	/* the amount of space we look for needs to be divisible by the standard size (Header) */
	cp = getMoreMem(nu * sizeof(Header));
	/* if sbrk can't find a large enough size, then it wouln't return a positive integer */
	if(cp == (char *) -1)
		return NULL;
	/* this returns a really large header, which can be broken down into a smaller pointers in the free linked list */
	up = (Header *) cp;
	up->s.size = nu;
	up->s.usedFlag = usedFlagValue;
	if(nu > 100) freep = &largefreep;
	/* the free function turns the header into multiple free linkedlist data structure */
	myfree((void *)(up+1));

	return *freep;
}

/*
*	getMoreMem is going to retrieve neededMemory amount of memory from the char pointer
*/
void *getMoreMem(int neededMemory)
{
	/* If you need a large chunk of memory, then the pointer to the large must point to a lower address because it starts
	from the right of the virtual memory*/
	if(neededMemory / sizeof(Header) > 100)
	{
		/* if decreasing this pointer will cause an overlap */
		if(largebreakpoint - neededMemory <= smallbreakpoint)	return -1;
		/* else change the location of the large breakpoint by the amount of space we need */
		else
		{
			largebreakpoint-=neededMemory;
			return &(virtualMemory+largebreakpoint);
		}
	}
		/* if increasing this pointer will cause an overlap */
	if(smallbreakpoint + neededMemory >= largebreakpoint)	return -1;
		/*else increase the limit at which the small blocks will be and return the start of the new block */
	else
	{
		smallbreakpoint+=neededMemory;
		return &virtualMemory[smallbreakpoint-neededMemory];
	}
}

/*
*	My implementation of realloc
*/
void *myRealloc(void *ptr, size_t size, const char file, const int line)
{
	
	Header *Headercpy = (Header *)ptr-1;

	/*If the pointer is null, then realloc works the same as Malloc */

	if (ptr == NULL)
	{
		return myMalloc(size, file, line);
	}

	/*
	*	Since the biggest of the new size and the old size becomes the size, we only change the size if the new size
	* is larger
	*/
	if(headercpy->s.size > size) return ptr;

	/*
	*	Now the only thing you need to do is get the new size. 
	*/
	void *ptrCpy = myMalloc(size, file, line);

	/*
	*	Copy the bit data into the new struct, so that it makes a complete copy.
	*/
	memcpy( ptrCpy , ptr , headercpy->s.size);
	/*
	*	free the old pointer
	*/
	myFree(ptr, file, line);
	/*
	*	return the new copy pointer with it's new, larger size
	*/
	return ptrCpy;
}
/*
*	This is my implementation of calloc, the difference between calloc and malloc is the memset
*/
void* myCalloc(size_t num, size_t size, const char file, const int line)
{
	void *tmpVar = myMalloc(num*size, file, line);

	memset(tmpVar, 0, num*size);

	return tmpVar;
}
/*
*	myfree is my own implementation of free function
*		the free function is going to deallocate a block of memory, then link the block to the free linked list
*	Input: pointer to what we need to be inserted into the free list
*/
void myfree(void * ap, const char line, const int line){
	Header *bp, *p;
	Header **freep;

	freep = &smallfreep;

	/* Since we need a pointer to the structure containing the object, we need to begin with address space 
	* ap-1, which is where we store the head */

	bp = (Header *)ap - 1;
	/* since we store the address spaces in terms of the relative size, we traverse the start of the linked list
	* to free blocks until we  reach a point where the address space is bigger than the previous block and smaller than the 
	* next link in the free block linked list */

	if(bp->s.usedFlag != usedFlagValue){
	 fprintf(stderr, "Error: The pointer you were trying to free in file:\"%s\" line :%d was never allocated\n", file, line);
	 return NULL;
	}
	/* If you're going to free a big block */
	if(bp->s.size > 100){
		/*reference the big block pointer */
		 freep = &largefreep;
		 /*go through the array until you find a spot where the address of bp is smaller than the address of the start of the link
		 *but bigger than the next item on the list */
	 	for(p = *freep; !(bp < p && bp > p->s.ptr); p = p->s.ptr))
		{
			if(p <= p->s.ptr && (bp < p|| bp > p->s.ptr))
				break;
			/* In the case that you free a pointer twice, then the function will print out the error and return NULL */
			if(bp == p || bp == p->s.ptr){
				fprintf(stderr, "Error: You have already freed pointer in file \"%s\" line: %d", file, line);
				return;
			}
		}
	 else{
		for(p = *freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
			/* in the case that both pointers are the same size, and the bp's address space is in between sizes for the p and it's
			* pointer */
			if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
				break;
	}
	/* once we've reached the right location*/
	/* if the block that is being freed is the same size as the next block, then instead of having 2 sizes variable for 2 blocks
	* you would be better off having more free space and having one size (aka combining both) */

	if( bp + bp->s.size == p->s.ptr){
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else /*if not then make the pointer of this header point to the next link */
		bp -> s.ptr = p -> s.ptr;
	/* If the pointer is just as large as bp then it makes a lot of sense to combine them into one block */
	if( p + p->s.size == bp){
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	}
	else /*else make the pointer point to the new block of memory */
		p->s.ptr= bp;

	/* now change the beginning of the linked list to the last place that got altered, so that in future free we can free addresses in memory
	that come after the one we freed and insert in the right spot */
	*freep = p;

}