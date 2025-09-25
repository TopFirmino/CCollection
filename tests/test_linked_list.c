#include "ihct.h"
#include "cc_linked_list.h"
#include <stdio.h>


CC_LL_DEFINE(MyList, MyItem, link,
    int  value;
    char name[16];
)


IHCT_TEST(basic_push_back) {
    MyList_head h;
    MyList_init(&h);

    MyItem a = {0}, b = {0}, c = {0};
    a.value = 0; b.value = 1; c.value = 2;

    MyList_push_back(&h, &a);
    MyList_push_back(&h, &b);
    MyList_push_back(&h, &c);

    int i = 0;
    for (MyItem *it = MyList_first(&h); it != 0; it = MyList_next(it)) {
        //printf("%d\n", it->value);
        IHCT_ASSERT( i++ == it->value);
    }
}


IHCT_TEST(basic_insert) {
    MyList_head h;
    MyList_init(&h);

    MyItem a = {0}, b = {0}, c = {0};
    a.value = 0; b.value = 1; c.value = 2;

    MyList_push_front(&h, &a);
    MyList_push_front(&h, &b);
    MyList_push_front(&h, &c);

    int i = 2;
    for (MyItem *it = MyList_first(&h); it != 0; it = MyList_next(it)) {
        //printf("%d\n", it->value);
        IHCT_ASSERT( i-- == it->value);
    }
}

IHCT_TEST(remove) {
    MyList_head h;
    MyList_init(&h);

    MyItem a = {0}, b = {0}, c = {0};
    a.value = 0; b.value = 1; c.value = 2;

    MyList_push_front(&h, &a);
    MyList_push_front(&h, &b);
    MyList_push_front(&h, &c);
    MyList_remove(&h, &b);

    int i = 2;
    for (MyItem *it = MyList_first(&h); it != 0; it = MyList_next(it)) {
        printf("\n%d\n", it->value);
        IHCT_ASSERT( i == it->value);
        i -= 2;
    }
}

IHCT_TEST(pop_front) {
    MyList_head h;
    MyList_init(&h);

    MyItem a = {0}, b = {0}, c = {0};
    a.value = 0; b.value = 1; c.value = 2;

    MyList_push_back(&h, &a);
    MyList_push_back(&h, &b);
    MyList_push_back(&h, &c);
    MyItem* popped = MyList_pop_front(&h);

    IHCT_ASSERT( popped != NULL );
    IHCT_ASSERT( popped->value == 0 );
    
    int i = 1;
    for (MyItem *it = MyList_first(&h); it != 0; it = MyList_next(it)) {
        printf("\n%d\n", it->value);
        IHCT_ASSERT( i++ == it->value);
    }
}


IHCT_TEST(pop_back) {
    MyList_head h;
    MyList_init(&h);

    MyItem a = {0}, b = {0}, c = {0};
    a.value = 0; b.value = 1; c.value = 2;

    MyList_push_back(&h, &a);
    MyList_push_back(&h, &b);
    MyList_push_back(&h, &c);
    MyItem* popped = MyList_pop_back(&h);

    IHCT_ASSERT( popped != NULL );
    IHCT_ASSERT( popped->value == 2 );

    int i = 0;
    for (MyItem *it = MyList_first(&h); it != 0; it = MyList_next(it)) {
        printf("\n%d\n", it->value);
        IHCT_ASSERT( i++ == it->value);
    }
}


IHCT_TEST(pop_at) {
    MyList_head h;
    MyList_init(&h);

    MyItem a = {0}, b = {0}, c = {0};
    a.value = 0; b.value = 1; c.value = 2;

    MyList_push_back(&h, &a);
    MyList_push_back(&h, &b);
    MyList_push_back(&h, &c);
    MyItem* popped = MyList_pop_at(&h, -1);

    IHCT_ASSERT( popped != NULL );
    IHCT_ASSERT( popped->value == 2 );

    int i = 0;
    for (MyItem *it = MyList_first(&h); it != 0; it = MyList_next(it)) {
        printf("\n%d\n", it->value);
        IHCT_ASSERT( i++ == it->value);
    }
}


IHCT_TEST(insert_at) {
    MyList_head h;
    MyList_init(&h);

    MyItem a = {0}, b = {0}, c = {0}, d = {0};
    a.value = 0; b.value = 1; c.value = 3; d.value = 2;

    MyList_push_back(&h, &a);
    MyList_push_back(&h, &b);
    MyList_push_back(&h, &c);
    MyList_insert_at(&h, &d, 2);

    int i = 0;
    for (MyItem *it = MyList_first(&h); it != 0; it = MyList_next(it)) {
        printf("\n%d\n", it->value);
        IHCT_ASSERT( i++ == it->value);
    }
}


/* struct list_item;

struct links {
    struct list_item *prev, *next;
};

typedef struct list_item{
    struct links MEMBER;
    int value;
} ListItem;



typedef struct list_head
{
    struct list_item *first, *last;
    size_t length;

} ListHead;


static inline void List_init(ListHead *h)                                                  
{ if (h != NULL) { h->first = NULL; h->last = NULL; h->length = 0U; } }              


static inline void List_push_back(ListHead *h, ListItem *obj)                                     
{                                                                                         
    if ((h == NULL) || (obj == NULL)) { return; }                                         
    obj->MEMBER.next = NULL;                                                            
    obj->MEMBER.prev = h->last;                                                           
    if (h->last != NULL) { h->last->MEMBER.next = obj; } else { h->first = obj; }         
    h->last = obj;                                                                         
    h->length++;                                                                           
}               

static inline ListItem * List_first(const ListHead *h)                                            
{ return (h != NULL) ? h->first : (ListItem *)0; }                                               
static inline ListItem * List_last(const ListHead *h)                                             
{ return (h != NULL) ? h->last : (ListItem *)0; }                                                
static inline ListItem * List_next(const ListItem *obj)                                                  
{ return (obj != NULL) ? obj->MEMBER.next : (ListItem *)0; }                                     
static inline ListItem * List_prev(const ListItem *obj)                                                  
{ return (obj != NULL) ? obj->MEMBER.prev : (ListItem *)0; }


IHCT_TEST() {
    ListHead h;
    List_init(&h);

    ListItem a = {0}, b = {0}, c = {0};
    a.value = 0; b.value = 1; c.value = 2;

    List_push_back(&h, &a);
    List_push_back(&h, &b);
    List_push_back(&h, &c);

    int i = 0;
    for (ListItem *it = List_first(&h); it != 0; it = List_next(it)) {
        printf("%d\n", it->value);
        IHCT_ASSERT( i++ == it->value);
    }

}
 */

int main(int argc, char **argv) {
    return IHCT_RUN(argc, argv);
}