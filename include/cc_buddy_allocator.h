/*

*/

/* cc_buddy_allocator.h */
#ifndef CC_BUDDY_ALLOCATOR_H
#define CC_BUDDY_ALLOCATOR_H

#include <stdlib.h>

/* Return Type */                                                                                          
typedef enum {                                                                                             
    CC_BUDDY_ALLOCATOR_SUCCESS = 0,                                                                         
    CC_BUDDY_ALLOCATOR_NOT_ALIGNED,                                                                         
    CC_BUDDY_ALLOCATOR_OUT_OF_RANGE,                                                                        
    CC_BUDDY_ALLOCATOR_NOT_ALLOCATED, 
} CC_BUDDY_AllocatorResult;  



#endif