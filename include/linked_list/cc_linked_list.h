#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Macro unica:
   P      = prefisso simboli (es. mylist)
   T      = nome del tipo elemento (es. MyItem)
   MEMBER = nome del campo link dentro T (es. link)
   ...    = campi utente di T (es. int value; char name[16];)

   Uso:
     CC_LL_DEFINE(MyList, MyItem, link,
         int value;
         char name[16];
     );
*/

#define CC_LL_DEFINE(P, T, MEMBER, ...)                                                       \
    typedef struct T T;                                                                       \
    struct P##_link { T *prev; T *next; };                                                    \
    struct T { struct P##_link MEMBER; __VA_ARGS__ };                                         \
    typedef struct { T *first; T *last; size_t length; } P##_head;                            \
                                                                                              \
    /* ==== helpers =================================================== */                     \
    static inline void P##_init(P##_head *h)                                                  \
    { if (h != NULL) { h->first = (T *)0; h->last = (T *)0; h->length = 0U; } }              \
                                                                                              \
    static inline bool P##_is_empty(const P##_head *h)                                        \
    { return ((h == NULL) || (h->length == 0U)); }                                            \
                                                                                              \
    static inline size_t P##_length(const P##_head *h)                                        \
    { return (h != NULL) ? h->length : 0U; }                                                  \
                                                                                              \
    /* Normalizza indice per POP: idx >=0 -> [0..len-1]; idx<0 -> len+idx ( -1 -> len-1 ) */  \
    static inline bool P##_normalize_pop_index(const P##_head *h, long idx, size_t *out_k)    \
    {                                                                                         \
        if ((h == NULL) || (out_k == NULL) || (h->length == 0U)) { return false; }            \
        if (idx >= 0L) {                                                                      \
            size_t k = (size_t)idx;                                                           \
            if (k >= h->length) { return false; }                                             \
            *out_k = k;                                                                       \
            return true;                                                                      \
        } else {                                                                              \
            long neg = -idx;                                                                  \
            size_t sneg = (size_t)neg;                                                        \
            if ((sneg == 0U) || (sneg > h->length)) { return false; }                         \
            *out_k = h->length - sneg;                                                        \
            return true;                                                                      \
        }                                                                                     \
    }                                                                                         \
                                                                                              \
    /* Normalizza indice per INSERT: idx>=0 -> [0..len]; idx<0 -> (len+1)+idx  (-1 -> len) */ \
    static inline bool P##_normalize_insert_index(const P##_head *h, long idx, size_t *out_k) \
    {                                                                                         \
        if ((h == NULL) || (out_k == NULL)) { return false; }                                 \
        if (idx >= 0L) {                                                                      \
            size_t k = (size_t)idx;                                                           \
            if (k > h->length) { return false; }                                              \
            *out_k = k;                                                                       \
            return true;                                                                      \
        } else {                                                                              \
            long neg = -idx;                                                                  \
            size_t sneg = (size_t)neg;                                                        \
            size_t maxn = h->length + 1U;                                                     \
            if ((sneg == 0U) || (sneg > maxn)) { return false; }                              \
            *out_k = maxn - sneg;                                                             \
            return true;                                                                      \
        }                                                                                     \
    }                                                                                         \
                                                                                              \
    /* Se k più vicino alla testa o alla coda, naviga di conseguenza */                       \
    static inline T * P##_node_at(const P##_head *h, size_t k)                                \
    {                                                                                         \
        if ((h == NULL) || (h->length == 0U) || (k >= h->length)) { return (T *)0; }          \
        if (k <= (h->length / 2U)) {                                                          \
            T *cur = h->first;                                                                \
            size_t i = 0U;                                                                    \
            while ((cur != NULL) && (i < k)) { cur = cur->MEMBER.next; i++; }                 \
            return cur;                                                                       \
        } else {                                                                              \
            T *cur = h->last;                                                                 \
            size_t i = h->length - 1U;                                                        \
            while ((cur != NULL) && (i > k)) { cur = cur->MEMBER.prev; i--; }                 \
            return cur;                                                                       \
        }                                                                                     \
    }                                                                                         \
                                                                                              \
    static inline void P##_link_between_(T *left, T *mid, T *right)                           \
    {                                                                                         \
        if (mid != NULL) { mid->MEMBER.prev = left; mid->MEMBER.next = right; }               \
        if (left != NULL) { left->MEMBER.next = mid; }                                        \
        if (right != NULL) { right->MEMBER.prev = mid; }                                      \
    }                                                                                         \
                                                                                              \
    /* ==== primitive base push/pop/remove ==================================== */            \
    static inline void P##_push_back(P##_head *h, T *obj)                                     \
    {                                                                                         \
        if ((h == NULL) || (obj == NULL)) { return; }                                         \
        obj->MEMBER.next = (T *)0;                                                            \
        obj->MEMBER.prev = h->last;                                                           \
        if (h->last != NULL) { h->last->MEMBER.next = obj; } else { h->first = obj; }         \
        h->last = obj;                                                                         \
        h->length++;                                                                           \
    }                                                                                         \
                                                                                              \
    static inline void P##_push_front(P##_head *h, T *obj)                                    \
    {                                                                                         \
        if ((h == NULL) || (obj == NULL)) { return; }                                         \
        obj->MEMBER.prev = (T *)0;                                                            \
        obj->MEMBER.next = h->first;                                                          \
        if (h->first != NULL) { h->first->MEMBER.prev = obj; } else { h->last = obj; }        \
        h->first = obj;                                                                        \
        h->length++;                                                                           \
    }                                                                                         \
                                                                                              \
    static inline T * P##_pop_front(P##_head *h)                                              \
    {                                                                                         \
        if ((h == NULL) || (h->first == NULL)) { return (T *)0; }                             \
        T *n = h->first;                                                                       \
        T *nx = n->MEMBER.next;                                                                \
        h->first = nx;                                                                         \
        if (nx != NULL) { nx->MEMBER.prev = (T *)0; } else { h->last = (T *)0; }              \
        n->MEMBER.next = (T *)0; n->MEMBER.prev = (T *)0;                                      \
        h->length--;                                                                           \
        return n;                                                                              \
    }                                                                                         \
                                                                                              \
    static inline T * P##_pop_back(P##_head *h)                                               \
    {                                                                                         \
        if ((h == NULL) || (h->last == NULL)) { return (T *)0; }                              \
        T *n = h->last;                                                                        \
        T *pv = n->MEMBER.prev;                                                                \
        h->last = pv;                                                                          \
        if (pv != NULL) { pv->MEMBER.next = (T *)0; } else { h->first = (T *)0; }             \
        n->MEMBER.next = (T *)0; n->MEMBER.prev = (T *)0;                                      \
        h->length--;                                                                           \
        return n;                                                                              \
    }                                                                                         \
                                                                                              \
    static inline void P##_remove(P##_head *h, T *obj)                                        \
    {                                                                                         \
        if ((h == NULL) || (obj == NULL)) { return; }                                         \
        T *pv = obj->MEMBER.prev;                                                              \
        T *nx = obj->MEMBER.next;                                                              \
        if (pv != NULL) { pv->MEMBER.next = nx; } else { h->first = nx; }                      \
        if (nx != NULL) { nx->MEMBER.prev = pv; } else { h->last = pv; }                       \
        if (h->length > 0U) { h->length--; }                                                   \
        obj->MEMBER.next = (T *)0; obj->MEMBER.prev = (T *)0;                                  \
    }                                                                                         \
                                                                                              \
    /* ==== insert_at / pop_at ================================================= */            \
    static inline bool P##_insert_at(P##_head *h, T *obj, long idx)                           \
    {                                                                                         \
        if ((h == NULL) || (obj == NULL)) { return false; }                                   \
        size_t k = 0U;                                                                         \
        if (!P##_normalize_insert_index(h, idx, &k)) { return false; }                        \
        if (k == 0U) { P##_push_front(h, obj); return true; }                                 \
        if (k == h->length) { P##_push_back(h, obj); return true; }                           \
        /* inserisci prima dell'elemento attualmente a posizione k */                         \
        T *pos = P##_node_at(h, k);                                                            \
        if (pos == NULL) { return false; }                                                    \
        T *left = pos->MEMBER.prev;                                                            \
        P##_link_between_(left, obj, pos);                                                     \
        h->length++;                                                                           \
        return true;                                                                           \
    }                                                                                         \
                                                                                              \
    static inline T * P##_pop_at(P##_head *h, long idx)                                       \
    {                                                                                         \
        if (h == NULL) { return (T *)0; }                                                     \
        size_t k = 0U;                                                                         \
        if (!P##_normalize_pop_index(h, idx, &k)) { return (T *)0; }                          \
        if (k == 0U) { return P##_pop_front(h); }                                             \
        if (k + 1U == h->length) { return P##_pop_back(h); }                                  \
        T *node = P##_node_at(h, k);                                                           \
        if (node == NULL) { return (T *)0; }                                                  \
        T *pv = node->MEMBER.prev;                                                             \
        T *nx = node->MEMBER.next;                                                             \
        if (pv != NULL) { pv->MEMBER.next = nx; }                                             \
        if (nx != NULL) { nx->MEMBER.prev = pv; }                                             \
        node->MEMBER.next = (T *)0; node->MEMBER.prev = (T *)0;                               \
        h->length--;                                                                           \
        return node;                                                                           \
    }                                                                                         \
                                                                                              \
    /* ==== accessori =========================================================== */           \
    static inline T * P##_first(const P##_head *h)                                            \
    { return (h != NULL) ? h->first : (T *)0; }                                               \
    static inline T * P##_last(const P##_head *h)                                             \
    { return (h != NULL) ? h->last : (T *)0; }                                                \
    static inline T * P##_next(const T *obj)                                                  \
    { return (obj != NULL) ? obj->MEMBER.next : (T *)0; }                                     \
    static inline T * P##_prev(const T *obj)                                                  \
    { return (obj != NULL) ? obj->MEMBER.prev : (T *)0; }



