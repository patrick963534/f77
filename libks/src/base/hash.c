#include <ks/hash.h>
#include <ks/libc.h>
#include <stdlib.h>
#include <string.h>

typedef struct item_t
{
    char*   key;
    void*   val;

    struct item_t* next;

} item_t;

struct ks_hash_t
{
    int      cap;
    item_t** items;
};

static item_t* search(ks_hash_t* me, int id, const char* key)
{
    item_t* t = me->items[id];

    while (t)
    {
        if (strcmp(t->key, key) == 0)
            return t;

        t = t->next;
    }

    return 0;
}

static int hash_string(const char* key)
{
    int h = 0;
    int i = 0;

    while (key[i++] != 0 && i < 6)
        h += key[i] * 7;

    if (h < 0)
        h = -h;

    return h;
}

KS_API ks_hash_t* ks_hash_new(int cap)
{
    ks_hash_t* me = (ks_hash_t*)malloc(sizeof(*me));
    me->items     = (item_t**)calloc(cap, sizeof(me->items[0]));
    me->cap       = cap;

    return me;
}

KS_API void ks_hash_add(ks_hash_t* me, const char* key, void* val)
{
    int i     = hash_string(key) % me->cap;
    item_t* t = search(me, i, key);

    if (t)
        return;

    t = calloc(1, sizeof(*t));
    t->key = ks_strdup(key);
    t->val = val;

    if (me->items[i])
        me->items[i]->next = t;
    else
        me->items[i] = t;
}