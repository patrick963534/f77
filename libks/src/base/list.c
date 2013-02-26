#include <ks/list.h>
#include <ks/log.h>

KS_API void ks_list_init(ks_list_t* head)
{
    head->next = head;
    head->prev = head;
}

KS_API void ks_list_add(ks_list_t* head, ks_list_t* node)
{
    ks_list_t *prev = head; 
    ks_list_t *next = head->next; 

    next->prev = node; 
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

KS_API void ks_list_add_tail(ks_list_t* head, ks_list_t* node)
{
    ks_list_t *prev = head->prev; 
    ks_list_t *next = head; 

    next->prev = node; 
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

KS_API void ks_list_remove(ks_list_t* node)
{
    ks_list_t *prev = node->prev; 
    ks_list_t *next = node->next; 

    prev->next = next;
    next->prev = prev;
}

KS_API void ks_list_remove_init(ks_list_t* node)
{
    ks_list_remove(node);

    ks_list_init(node);
}

KS_API void ks_list_insert_after(ks_list_t* pos, ks_list_t* node)
{
    ks_list_t *prev = pos; 
    ks_list_t *next = pos->next; 

    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

KS_API void ks_list_insert_before(ks_list_t* pos, ks_list_t* node)
{
    ks_list_t *prev = pos->prev; 
    ks_list_t *next = pos; 

    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}
