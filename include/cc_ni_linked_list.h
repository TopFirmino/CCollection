/* cc_ni_linked_list.h — A minimalist implementation of a non intrusive doubly-linked list that works only with handlers. 
 * The lists does not store data in any way. It justs keeps the links and hosts API to navigate thee user defined arrays.
 * Users shall manage the memory by itself. A suggested strategy is to use the cc_slab_allocator.
*/

#ifndef CC_ARRAY_DLIST_H
#define CC_ARRAY_DLIST_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef CC_LL_INVALID
#define CC_LL_INVALID (SIZE_MAX)
#endif

typedef size_t CCNodeHandle;    /* handle del NODO nella lista (indice tabella nodi) */
typedef size_t CCPayloadHandle; /* handle del PAYLOAD utente (indice nel buffer utente) */

typedef enum {
    CC_LL_OK = 0,
    CC_LL_ERR_PARAM,
    CC_LL_ERR_EMPTY,
    CC_LL_ERR_OUT_OF_RANGE,
    CC_LL_ERR_NODE_USED,       /* node già in lista */
    CC_LL_ERR_NODE_FREE,       /* nodo non in lista quando ci si aspetta che lo sia */
    CC_LL_ERR_STATE
} CCListStatus;

/* Nodo della lista: prev/next sono HANDLE DI NODO; payload è HANDLE DELL’UTENTE. */
typedef struct {
    CCNodeHandle    prev;       /* CC_LL_INVALID se none */
    CCNodeHandle    next;       /* CC_LL_INVALID se none */
    CCPayloadHandle payload;    /* handle nel buffer utente */
} CCListNode;

/* Testa della d-list. head/tail sono HANDLE DI NODO. */
typedef struct {
    CCNodeHandle head;      /* CC_LL_INVALID se vuota */
    CCNodeHandle tail;      /* CC_LL_INVALID se vuota */
    size_t       size;      /* numero di nodi linkati */
    size_t       capacity;  /* numero massimo di nodi gestibili (lunghezza di nodes[]) */
    CCListNode*  nodes;     /* array di nodi (fornito dal chiamante) */
} CCListHead;

/* ===================== API ===================== */

/* Inizializza la lista vuota e azzera i campi dei nodi.
 * Requisiti: head != NULL, nodes != NULL, capacity > 0
 */
CCListStatus cc_ll_init(CCListHead* head, size_t capacity, CCListNode* nodes);

/* Inserimenti: il chiamante fornisce un node_handle LIBERO e il payload_handle da linkare.
 * Requisiti: node < capacity, nodes[node].used == 0
 */
CCListStatus cc_ll_push_back (CCListHead* head, CCNodeHandle node, CCPayloadHandle payload);
CCListStatus cc_ll_push_front(CCListHead* head, CCNodeHandle node, CCPayloadHandle payload);

/* Pop: rimuovono head/tail e restituiscono sia il node_handle sia il payload_handle. */
CCListStatus cc_ll_pop_front(CCListHead* head, CCNodeHandle* out_node, CCPayloadHandle* out_payload);
CCListStatus cc_ll_pop_back (CCListHead* head, CCNodeHandle* out_node, CCPayloadHandle* out_payload);

/* at: ritorna il node_handle e il payload_handle alla posizione 'index' (0-based). */
CCListStatus cc_ll_at(const CCListHead* head, size_t index, CCNodeHandle* out_node, CCPayloadHandle* out_payload);

/* Utility opzionali */
CCListStatus cc_ll_is_empty(const CCListHead* head, bool* out_is_empty);
CCListStatus cc_ll_size    (const CCListHead* head, size_t* out_size);

#endif /* CC_ARRAY_DLIST_H */
