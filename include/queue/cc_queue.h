/* cc_fifo_one.h */
#ifndef CC_FIFO_ONE_H
#define CC_FIFO_ONE_H

#include <stddef.h>
#include <stdbool.h>

/* Macro unica:
   P      = prefisso simboli (es. MyQ)
   T      = nome del tipo elemento (es. MyItem)
   MEMBER = nome del campo link dentro T (es. link)
   ...    = campi utente di T (es. int value;)

   Uso:
     CC_Q_DEFINE(MyQ, MyItem, link,
         int value;
     );
*/
#define CC_Q_DEFINE(P, T, MEMBER, ...)                                                      \
    typedef struct T T;                                                                     \
    struct P##_link { T *next; };                                                           \
    struct T { struct P##_link MEMBER; __VA_ARGS__ };                                       \
                                                                                            \
    typedef struct {                                                                        \
        T     *first;                                                                       \
        T     *last;                                                                        \
        size_t length;                                                                      \
    } P##_head;                                                                             \
                                                                                            \
    /* ---- helpers ---- */                                                                 \
    static inline void P##_init(P##_head *q)                                                \
    { if (q != NULL) { q->first = (T *)0; q->last = (T *)0; q->length = 0U; } }            \
                                                                                            \
    static inline bool P##_is_empty(const P##_head *q)                                      \
    { return ((q == NULL) || (q->length == 0U)); }                                          \
                                                                                            \
    static inline size_t P##_length(const P##_head *q)                                      \
    { return (q != NULL) ? q->length : 0U; }                                                \
                                                                                            \
    static inline T * P##_peek_front(const P##_head *q)                                     \
    { return (q != NULL) ? q->first : (T *)0; }                                             \
                                                                                            \
    static inline T * P##_peek_back(const P##_head *q)                                      \
    { return (q != NULL) ? q->last : (T *)0; }                                              \
                                                                                            \
    static inline T * P##_next(const T *obj)                                                \
    { return (obj != NULL) ? obj->MEMBER.next : (T *)0; }                                   \
                                                                                            \
    /* ---- enqueue (push in coda) ---- */                                                  \
    static inline void P##_enqueue(P##_head *q, T *obj)                                     \
    {                                                                                       \
        if ((q == NULL) || (obj == NULL)) { return; }                                       \
        /* difensivo: non inserire se già linkato */                                        \
        if (obj->MEMBER.next != (T *)0) { return; }                                         \
        obj->MEMBER.next = (T *)0;                                                          \
        if (q->last != (T *)0) {                                                            \
            q->last->MEMBER.next = obj;                                                     \
        } else {                                                                            \
            q->first = obj;                                                                 \
        }                                                                                   \
        q->last = obj;                                                                       \
        q->length++;                                                                         \
    }                                                                                       \
                                                                                            \
    /* ---- dequeue (pop dalla testa) ---- */                                               \
    static inline T * P##_dequeue(P##_head *q)                                              \
    {                                                                                       \
        if ((q == NULL) || (q->first == (T *)0)) { return (T *)0; }                         \
        T *n = q->first;                                                                     \
        q->first = n->MEMBER.next;                                                           \
        if (q->first == (T *)0) { q->last = (T *)0; }                                       \
        n->MEMBER.next = (T *)0;                                                            \
        q->length--;                                                                         \
        return n;                                                                            \
    }                                                                                       \
                                                                                            \
    /* ---- clear: svuota la coda, azzera i link ---- */                                    \
    static inline void P##_clear(P##_head *q)                                               \
    {                                                                                       \
        if (q == NULL) { return; }                                                          \
        T *cur = q->first;                                                                   \
        while (cur != (T *)0) {                                                             \
            T *nx = cur->MEMBER.next;                                                       \
            cur->MEMBER.next = (T *)0;                                                      \
            cur = nx;                                                                        \
        }                                                                                   \
        q->first = (T *)0; q->last = (T *)0; q->length = 0U;                                 \
    }                                                                                       \
                                                                                            \
    /* ---- splice: concatena src in coda a dst (O(1)) ---- */                              \
    static inline void P##_splice(P##_head *dst, P##_head *src)                             \
    {                                                                                       \
        if ((dst == NULL) || (src == NULL) || (src->length == 0U)) { return; }              \
        if (dst->length == 0U) {                                                            \
            dst->first = src->first;                                                        \
            dst->last  = src->last;                                                         \
        } else {                                                                            \
            dst->last->MEMBER.next = src->first;                                            \
            dst->last = src->last;                                                          \
        }                                                                                   \
        dst->length += src->length;                                                         \
        src->first = (T *)0; src->last = (T *)0; src->length = 0U;                          \
    }

#endif /* CC_FIFO_ONE_H */
