#include <stdio.h>
#include <stdint.h>

typedef struct {

    int value;

} AllocatorItem;

typedef struct {

    AllocatorItem *base;

    uint32_t first;
    uint32_t *freelist;

    uint32_t max_length; // Number of items
    uint32_t size; // Total size of the memory

} Allocator;

const static uint32_t Allocator_last_index = -1;
const static uint32_t Allocator_detached_index = -2;

typedef enum {
    SUCCESS = 0,
    NOT_ALIGNED,
    OUT_OF_RANGE,
    NOT_ALLOCATED,

} AllocatorResult;

static inline void Allocator_init(Allocator *a, AllocatorItem *b, uint32_t *fl, uint32_t ml )
{
    uint32_t i = 0;

    a->base = b;
    a->freelist = fl;
    a->max_length = ml;
    a->first = 0;
    
    for ( i = 0; i < ml; ++i )
    {
        a->freelist[i] = i+1;
    }

    a->freelist[ml-1] = Allocator_last_index;
}

static inline AllocatorItem* Allocator_allocate(Allocator *a) {

    uint32_t detached_index = a->first; 

    if ( a->first == Allocator_last_index )
    {
        return 0;
    }
    
    a->first = a->freelist[a->first];
    --a->size;
    a->freelist[detached_index] = Allocator_detached_index;

    return &(a->base[detached_index]);
}

static inline AllocatorResult Allocator_free( Allocator *a, AllocatorItem *item)
{
    uint32_t offset = item - a->base;
    
    printf("Free offset %d\n", offset);

    if ( offset<0 || offset>=a->max_length ){ return OUT_OF_RANGE; }   // OutOfRange

    if ( a->freelist[offset] != Allocator_detached_index ){ return NOT_ALLOCATED; } // DoubleFree

    a->freelist[offset] = a->first;
    a->first = offset; 
    ++a->size;

    return SUCCESS;
}


int main()
{

    Allocator all;
    AllocatorItem items[4];
    uint32_t freelist[4];

    AllocatorItem *tests[4];

    items[0].value = 1;
    items[1].value = 2;
    items[2].value = 3;
    items[3].value = 4;

    Allocator_init( &all, items, freelist, 4);

    AllocatorItem *item = Allocator_allocate( &all );

    printf("Shall be 1: %d\n", item->value);
    printf("Shall be not null %p\n", item );

    Allocator_free(&all, item);

    item = Allocator_allocate( &all );
    
    
    printf("Shall be 1: %d\n", item->value);
    printf("Shall be not null %p\n", item );
    
    Allocator_free(&all, item);

    for (int i = 0; i < 4; ++i)
    {
        tests[i] = Allocator_allocate( &all );
        
        printf("Shall be %d: %d\n", i + 1, tests[i]->value);
        printf("Shall be not null %p\n", tests[i] );
    }

    int res = Allocator_free(&all, tests[2]);
    printf("Free shall be ok: %d\n", res);

    item = Allocator_allocate( &all );

    printf("Shall be not null %p\n", item );
    printf("Shall be %d: %d\n", 3, item->value);

}



