#include "cc_slab_allocator.h"
#include <stdio.h>

typedef struct {
    int val;
} MyType;

CC_SLAB_ALLOCATOR_DEFINE( MyType, 4 );


int main()
{

    MyTypeAllocator allocator;
    MyType buffer[4];
    MyType *tests[4];

    MyTypeAllocator_init( &allocator, buffer );
    tests[0] = MyTypeAllocator_allocate( &allocator );
    tests[0]->val = 3;

    MyTypeAllocator_free( &allocator, tests[0] );

    tests[1] = MyTypeAllocator_allocate( &allocator );
    
    printf("Shall be 3:%d\n", tests[1]->val);


    return 0;

}