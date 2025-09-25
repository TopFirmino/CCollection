/* cc_slab_allocator.h */
#ifndef CC_SLAB_ALLOCATOR_H
#define CC_SLAB_ALLOCATOR_H

#include <stdint.h>


/* T is the Item Type */                                                                                   \
#define CC_SLAB_ALLOCATOR_DEFINE( ITEM_T, NUMBER )                                                         \
typedef struct {                                                                                           \
    ITEM_T *base;                                                                                          \
    uint32_t first;                                                                                        \
    uint32_t *freelist;                                                                                    \
    uint32_t max_length;                                                                                   \
    uint32_t size;                                                                                         \
} ITEM_T##Allocator;                                                                                       \
                                                                                                           \
/* Constants */                                                                                            \
const static uint32_t ITEM_T##ALLOCATOR_LAST_IDX = -1;                                                     \
const static uint32_t ITEM_T##ALLOCATOR_DETACHED_IDX = -2;                                                 \
                                                                                                           \
/* Variables */                                                                                            \
static uint32_t ITEM_T##ALLOCATOR_freelist[NUMBER];                                                        \
                                                                                                           \
/* Return Type */                                                                                          \
typedef enum {                                                                                             \
    ITEM_T##ALLOCATOR_SUCCESS = 0,                                                                         \
    ITEM_T##ALLOCATOR_NOT_ALIGNED,                                                                         \
    ITEM_T##ALLOCATOR_OUT_OF_RANGE,                                                                        \
    ITEM_T##ALLOCATOR_NOT_ALLOCATED,                                                                       \
} ITEM_T##AllocatorResult;                                                                                 \
                                                                                                           \
/* APIs */                                                                                                 \
static inline void ITEM_T##Allocator_init(ITEM_T##Allocator *a, ITEM_T *b )                                \
{                                                                                                          \
    uint32_t i = 0;                                                                                        \
                                                                                                           \
    a->base = b;                                                                                           \
    a->freelist = ITEM_T##ALLOCATOR_freelist;                                                              \
    a->max_length = NUMBER;                                                                                \
    a->first = 0;                                                                                          \
                                                                                                           \
    for ( i = 0; i < NUMBER; ++i )                                                                         \
    {                                                                                                      \
        a->freelist[i] = i+1;                                                                              \
    }                                                                                                      \
                                                                                                           \
    a->freelist[NUMBER-1] = ITEM_T##ALLOCATOR_LAST_IDX;                                                    \
}                                                                                                          \
                                                                                                           \
static inline ITEM_T* ITEM_T##Allocator_allocate(ITEM_T##Allocator *a) {                                   \
                                                                                                           \
    uint32_t detached_index = a->first;                                                                    \
                                                                                                           \
    if ( a->first == ITEM_T##ALLOCATOR_LAST_IDX )                                                          \
    {                                                                                                      \
        return 0;                                                                                          \
    }                                                                                                      \
                                                                                                           \
    a->first = a->freelist[a->first];                                                                      \
    --a->size;                                                                                             \
    a->freelist[detached_index] = ITEM_T##ALLOCATOR_DETACHED_IDX;                                          \
                                                                                                           \
    return &(a->base[detached_index]);                                                                     \
}                                                                                                          \
                                                                                                           \
static inline ITEM_T##AllocatorResult ITEM_T##Allocator_free( ITEM_T##Allocator *a, ITEM_T *item)          \
{                                                                                                          \
    uint32_t offset = item - a->base;                                                                      \
                                                                                                           \
                                                                                                           \
    if ( offset<0 || offset>=a->max_length ){ return ITEM_T##ALLOCATOR_OUT_OF_RANGE; }                     \
                                                                                                           \
    if ( a->freelist[offset] != ITEM_T##ALLOCATOR_DETACHED_IDX ){ return ITEM_T##ALLOCATOR_NOT_ALLOCATED; }\
                                                                                                           \
    a->freelist[offset] = a->first;                                                                        \
    a->first = offset;                                                                                     \
    ++a->size;                                                                                             \
                                                                                                           \
    return ITEM_T##ALLOCATOR_SUCCESS;                                                                      \
}                                                                                                          
                                                                                                                          
#endif