#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

#define HASHTABLE_SEARCH_VALUE(_ht, _key, _T)                                  \
    (*(_T *)hashtable_search(_ht, _key))

#define HASHTABLE_SEARCH_POINTER(_ht, _key, _T)                                \
    ((_T *)hashtable_search(_ht, _key))

typedef struct HTEntry HTEntry;
typedef struct HTEntry
{
    void *key;
    void *value;
    void *next_ptr;
    void (*freekey)(void *k);
    void (*freeval)(void *v);
} HTEntry;

typedef struct HashTable
{
    HTEntry **table; /* pointer to table array */
    unsigned int size;
    unsigned int entry_count;
    float max_loadfactor;
    unsigned short delta_idx;
    size_t (*hash)(void *key, unsigned int table_size);
    int (*cmp)(void *key_A, void *key_B);
} HashTable;

HashTable *hashtable_create(float loadfactor,
                            size_t (*hash)(void *, unsigned int),
                            int (*cmp)(void *, void *));
int hashtable_free(HashTable *ht);

int hashtable_insert(HashTable *ht, void *key, void *value);
int hashtable_insert_pro(HashTable *ht, void *key, void *value,
                         void (*freekey)(void *k), void (*freeval)(void *val));

void *hashtable_search(HashTable *ht, void *key);
int hashtable_remove(HashTable *ht, void *key);

#endif /* HASHTABLE_H */
