#ifndef __KS_LIST_H__
#define __KS_LIST_H__

#include <ks/defs.h>

typedef struct ks_list_t
{
    struct ks_list_t*   next;
    struct ks_list_t*   prev;
} ks_list_t;

KS_API void ks_list_init(ks_list_t* head);KS_API void ks_list_add(ks_list_t* head, ks_list_t* node);KS_API void ks_list_add_tail(ks_list_t* head, ks_list_t* node);KS_API void ks_list_remove(ks_list_t* node);KS_API void ks_list_remove_init(ks_list_t* node);KS_API void ks_list_insert_after(ks_list_t* pos, ks_list_t* node);KS_API void ks_list_insert_before(ks_list_t* pos, ks_list_t* node);
#define ks_list_entry(ptr, type, member) \    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))
#define ks_list_empty(head) \    ((head)->next == (head))
#define ks_list_next_entry(ptr, type, member)   \    ks_list_entry((ptr)->member.next, type, member)
#define ks_list_first_entry(head, type, member) \    ks_list_entry((head)->next, type, member)
#define ks_list_last_entry(head, type, member)  \    ks_list_entry((head)->prev, type, member)

#define ks_list_for_each_entry(pos, head, type, member)     \    for (pos = ks_list_entry((head)->next, type, member);   \         &pos->member != (head);                            \          pos = ks_list_entry(pos->member.next, type, member))
#define ks_list_for_each_entry_safe(pos, n, head, type, member)     \    for (pos = ks_list_entry((head)->next, type, member),           \          n = ks_list_entry(pos->member.next, type, member);        \         &pos->member != (head);                                    \          pos = n, n = ks_list_entry(n->member.next, type, member))

#define ks_list_for_each_entry_reverse(pos, head, type, member)     \    for (pos = ks_list_entry((head)->prev, type, member);   \         &pos->member != (head);                            \          pos = ks_list_entry(pos->member.prev, type, member))
#define ks_list_for_each_entry_reverse_safe(pos, n, head, type, member)     \    for (pos = ks_list_entry((head)->prev, type, member),           \          n = ks_list_entry(pos->member.prev, type, member);        \         &pos->member != (head);                                    \          pos = n, n = ks_list_entry(n->member.prev, type, member))

#endif