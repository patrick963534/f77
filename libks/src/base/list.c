#include <ks/list.h>

KS_API void ks_list_init(ks_list_t* head)
{
    head->next = head;
    head->prev = head;
}
