#include "dynamic.h"

#define NALLOC 1024;

static Header base; /* This is the empty list */
static Header *freep = NULL;	/*this is where the free list will begin */

/*
*	myMalloc is supposed to return a pointer to a free space in memory, so that segmentation faults become impossible. 
*/

void *myMalloc(unsigned nBytes){

	Header *p, *prevp;
	Header *myMorecore(unsigned);
	unsigned nunits;

	/* nunits is going to be the size of the block that we need */
	nunits = (nBytes+sizeof(Header) - 1)/sizeof(Header) + 1;
	/* since we need the block to be a multiple of Header we divide the nBytes by header size
	the + 1 is for the size*/

	/* This is the case in which the free linked list is empty, here the size is naturally 0 and the ptr points to itself*/
	if((prevp = freep) == NULL){
		base.s.ptr = freep = prevp = &base;
		base.s.size = 0;
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

			freep = prevp; /*freep points to the beginning of the free linked list structure */
			return (void*) (p+1); /* since we use 1 for the size, we return a pointer to where the structure will be hold */
		}
		if ( p == freep) /* This is the case in which we cannot find a space large enough */
			if((p = myMorecore(nunits)) == NULL) /* myMoreCore will find more space, but if it can't then we return NULL */
				return NULL;
	}
}

/*
*	myMorecore is supposed to ask the system for more memory
*/
static Header * myMorecore(unsigned nu){

	char *cp, *sbrk(int);
	Header *up;

	/* NALLOC is the minimum amount of space that we want to look for
	* we want to minimize the amount of systemCalls we make because they're intensive on the runTime, so asking for more 
	* makes us ask less times */
	if(nu < NALLOC)
		nu = NALLOC;
	/* the amount of space we look for needs to be divisible by the standard size (Header) */
	cp = sbrk(nu * sizeof(Header));
	/* if sbrk can't find a large enough size, then it wouln't return a positive integer */
	if(cp == (char *) -1)
		return NULL;
	/* this returns a really large header, which can be broken down into a smaller pointers in the free linked list */
	up = (Header *) cp;
	up->s.size = nu;
	/* the free function turns the header into multiple free linkedlist data structure */
	myfree((void *)(up+1));

	return freep;
}
/*
*	myfree is my own implementation of free function
*		the free function is going to deallocate a block of memory, then link the block to the free linked list
*	Input: pointer to what we need to be inserted into the free list
*/
void myfree(void * ap){
	Header *bp, *p;
	/* Since we need a pointer to the structure containing the object, we need to begin with address space 
	* ap-1, which is where we store the head */

	bp = (Header *)ap - 1;
	/* since we store the address spaces in terms of the relative size, we traverse the start of the linked list
	* to free blocks until we  reach a point where the address space is bigger than the previous block and smaller than the 
	* next link in the free block linked list */
	for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr){
		/* in the case that both pointers are the same size, and the bp's address space is in between sizes for the p and it's
		* pointer */
		if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
			break;
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
	freep = p;

}