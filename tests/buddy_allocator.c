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

static inline uint32_t first_idx(uint8_t level)
{
    return 1 << level;
}

static inline uint32_t node_offset_in_level(uint32_t index)
{
    return index - first_idx(level_from_index(index));
}

static inline uint32_t buddy_index(uint32_t index)
{
    return (index%2) * (index - 1) + (1 - (index%2)) * (index + 1);
}

static inline uint32_t parent_index(uint32_t index)
{
    return floor(index / 2);
}


int main()
{



    return 0;
}