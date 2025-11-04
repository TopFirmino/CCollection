#include "cc_buddy_allocator.h"

static inline size_t level_from_index(size_t i)
{
    return floor(log2(i));
}

static inline size_t first_idx(size_t level)
{
    return 1 << level;
}

static inline size_t node_offset_in_level(size_t index)
{
    return index - first_idx(level_from_index(index));
}

static inline size_t buddy_index(size_t index)
{
    return (index%2) * (index - 1) + (1 - (index%2)) * (index + 1);
}

static inline size_t parent_index(size_t index)
{
    return floor(index / 2);
}
