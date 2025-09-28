#include "cc_slab_allocator.h"

#define CC_SLAB_ALLOCATOR_LAST_INDEX -1         /* Represents the last index of the arraylist. Used in order to undestand if the memory is over. */
#define CC_SLAB_ALLOCATOR_DETACHED_INDEX -2     /* Represents the index for a detached index. Used to understand if a slot is already used */

void cc_slab_alloctor_init( CC_SLAB_Allocator *a, size_t *freelist, size_t size)
{
    size_t i = 0;
    
    // Init the arraylist
    a->first = 0;
    a->freelist = freelist;
    a->size = size;

    for (i = 0; i < size; ++i)
    {
        a->freelist[i] = i+1;
    }

    a->freelist[size - 1] = CC_SLAB_ALLOCATOR_LAST_INDEX;
}

size_t cc_slab_allocator_alloc( CC_SLAB_Allocator *a)
{
    size_t detached_index = a->first;

    if ( a->first == CC_SLAB_ALLOCATOR_LAST_INDEX)                                                          
    {                                                                                                      
        return -1; /* OUT OF MEMORY */                                                                                         
    } 

    a->first = a->freelist[a->first];
    a->freelist[detached_index] = CC_SLAB_ALLOCATOR_DETACHED_INDEX;

    return detached_index;
}


CC_SLAB_AllocatorResult cc_slab_allocator_free( CC_SLAB_Allocator *a, size_t index)
{
    if ( index>=a->size )
    { 
        return CC_SLAB_ALLOCATOR_OUT_OF_RANGE; 
    }

    if ( a->freelist[index] != CC_SLAB_ALLOCATOR_DETACHED_INDEX )
    { 
        return CC_SLAB_ALLOCATOR_NOT_ALLOCATED; 
    }

    a->freelist[index] = a->first; 
    a->first = index;

    return CC_SLAB_ALLOCATOR_SUCCESS;
}
