/* cc_stack_one.h */
#ifndef CC_STACK_ONE_H
#define CC_STACK_ONE_H

#include <stddef.h>
#include <stdbool.h>

/* Macro unica:
   P      = prefisso dei simboli (es. MyStack)
   T      = nome del tipo elemento (es. MyItem)
   MEMBER = nome del campo link dentro T (es. link)
   ...    = campi utente di T (es. int value;)

   Uso:
     CC_STACK_DEFINE(MyStack, MyItem, link,
         int value;
     );
*/
#define CC_STACK_DEFINE(P, T, MEMBER, ...)                                                   \
    typedef struct T T;                                                                      \
    struct P##_link { T *next; };                                                            \
    struct T { struct P##_link MEMBER; __VA_ARGS__ };                                        \
                                                                                             \
    typedef struct {                                                                          \
        T     *top;                                                                           \
        size_t length;                                                                        \
    } P##_head;                                                                               \
                                                                                             \
    /* ---- helpers ---- */                                                                  \
    static inline void P##_init(P##_head *s)                                                 \
    { if (s != NULL) { s->top = (T *)0; s->length = 0U; } }                                  \
                                                                                             \
    static inline bool P##_is_empty(const P##_head *s)                                       \
    { return ((s == NULL) || (s->length == 0U)); }                                           \
                                                                                             \
    static inline size_t P##_length(const P##_head *s)                                       \
    { return (s != NULL) ? s->length : 0U; }                                                 \
                                                                                             \
    static inline T * P##_peek(const P##_head *s)                                            \
    { return (s != NULL) ? s->top : (T *)0; }                                                \
                                                                                             \
    static inline T * P##_next(const T *obj)                                                 \
    { return (obj != NULL) ? obj->MEMBER.next : (T *)0; }                                    \
                                                                                             \
    /* ---- push: inserisce in testa ---- */                                                 \
    static inline void P##_push(P##_head *s, T *obj)                                         \
    {                                                                                        \
        if ((s == NULL) || (obj == NULL)) { return; }                                        \
        /* Difensivo: evita doppia inserzione di un nodo ancora linkato */                   \
        if (obj->MEMBER.next != (T *)0) { return; }                                          \
        obj->MEMBER.next = s->top;                                                           \
        s->top = obj;                                                                        \
        s->length++;                                                                         \
    }                                                                                        \
                                                                                             \
    /* ---- pop: rimuove dalla testa ---- */                                                 \
    static inline T * P##_pop(P##_head *s)                                                   \
    {                                                                                        \
        if ((s == NULL) || (s->top == (T *)0)) { return (T *)0; }                            \
        T *n = s->top;                                                                       \
        s->top = n->MEMBER.next;                                                             \
        n->MEMBER.next = (T *)0;                                                             \
        s->length--;                                                                         \
        return n;                                                                            \
    }                                                                                        \
                                                                                             \
    /* ---- clear: svuota lo stack, azzera next di ogni nodo ---- */                         \
    static inline void P##_clear(P##_head *s)                                                \
    {                                                                                        \
        if (s == NULL) { return; }                                                           \
        T *cur = s->top;                                                                     \
        while (cur != (T *)0) {                                                              \
            T *nx = cur->MEMBER.next;                                                        \
            cur->MEMBER.next = (T *)0;                                                       \
            cur = nx;                                                                         \
        }                                                                                    \
        s->top = (T *)0;                                                                     \
        s->length = 0U;                                                                      \
    }                                                                                        \
                                                                                             \
    /* ---- splice: sposta tutto src sopra dst (preserva ordine LIFO) ---- */                \
    /* Implementazione O(n): pop da src -> push su dst, così il top di src diventa           \
       il nuovo top di dst, mantenendo l'ordine di pop originale. */                          \
    static inline void P##_splice(P##_head *dst, P##_head *src)                              \
    {                                                                                        \
        if ((dst == NULL) || (src == NULL) || (src->length == 0U)) { return; }               \
        T *n = (T *)0;                                                                       \
        while ((n = P##_pop(src)) != (T *)0) {                                               \
            /* n->next è già NULL dopo pop */                                                \
            P##_push(dst, n);                                                                \
        }                                                                                    \
    }

#endif /* CC_STACK_ONE_H */
