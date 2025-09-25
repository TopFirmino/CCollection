#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef struct {
    int value;

} BuddyItem;

typedef struct {



} BuddyAllocator;


static inline uint8_t level_from_index(uint32_t i)
{
    return floor(log2(i));
}

static inline uint32_t first_node_index_from_level(uint8_t level)
{
    return 1 << level;
}

static inline uint32_t node_offset_in_level(uint32_t index)
{
    return index - first_node_index_from_level(level_from_index(index));
}

int main()
{



    return 0;
}