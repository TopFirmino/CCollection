/* 
A minamalistic implementation of a slab allocator with only static allocation. Instead of returning an address as usually, this allocator only works with offsets.
This particulary useful for handling external memory devices and for handling allocations for an arrays of elements.

*/


/* cc_slab_allocator.h */
#ifndef CC_SLAB_ALLOCATOR_H
#define CC_SLAB_ALLOCATOR_H

#include <stdlib.h>

/* Return Type */                                                                                          
typedef enum {                                                                                             
    CC_SLAB_ALLOCATOR_SUCCESS = 0,                                                                         
    CC_SLAB_ALLOCATOR_NOT_ALIGNED,                                                                         
    CC_SLAB_ALLOCATOR_OUT_OF_RANGE,                                                                        
    CC_SLAB_ALLOCATOR_NOT_ALLOCATED, 
} CC_SLAB_AllocatorResult;  


typedef struct {
    size_t first;                                                                                        
    size_t *freelist;                                                                                  
    size_t size;                                                                                   
} CC_SLAB_Allocator;


/*  
    Init function for the slab allocator. 

    Parameters:
        CC_SLAB_Allocator* a: is a pointer to the slab allocator. 
        size_t *freelist: if a point to an array of size_t elements of size "size" 
        size_t size: is the maximum number of memory slots. 

    In order to keep the memory allocation static it is necessary for the user to allocate the memory for internal data structures by its own.
*/
void cc_slab_alloctor_init( CC_SLAB_Allocator *a, size_t *freelist, size_t size);

/*  
    Allocate function for the slab allocator.
    Parameters:
        CC_SLAB_Allocator* a: is a pointer to the slab allocator. 
    
    This function returns a free memory slot.
*/
size_t cc_slab_allocator_alloc( CC_SLAB_Allocator *a);


/*
    Free function for the slab allocator.

    Parameters:
        CC_SLAB_Allocator* a: is a pointer to the slab allocator. 
        size_t index: the index of the memory block to free.
        
    Returns a CC_SLAB_AllocatorResult.
*/
CC_SLAB_AllocatorResult cc_slab_allocator_free( CC_SLAB_Allocator *a, size_t index);


#endif