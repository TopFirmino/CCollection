/*

A minimalist implementation of a buddy allocator with only static allocation. Instead of returning an address as usually, this allocator only works with offsets.
This is particularly useful for handling external memory devices and for handling allocations for an arrays of elements.
The implementation of the buddy allocator is based on the index enumerator that enables to calculates easily all the parameters for the binary tree.
The current version does not use a bitmap.

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


typedef struct cc_buddy_item {

    size_t index; 
    struct cc_buddy_item *parent;
    struct cc_buddy_item *buddy;

} CCBuddyItem;


#endif