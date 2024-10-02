#include "blackboard.h"
#include <string.h>
#include <stdlib.h>

static const float LOAD_FACTOR = 0.5f;

static size_t _blackboard_hash(void *path, unsigned int table_size);
static int _cmp_keys(void *a, void *b);

Blackboard *blackboard_create()
{
    Blackboard *bb = (Blackboard *)malloc(sizeof(Blackboard));
    bb->table = hashtable_create(
        LOAD_FACTOR,
        _blackboard_hash,
        _cmp_keys
    );

    return bb;
}

void blackboard_free(Blackboard *bb)
{
    hashtable_free(bb->table);
    free(bb);
}

int blackboard_insert(Blackboard *bb, const char *key, void *value)
{
    char *str_key = (char *)malloc(sizeof(char) * BLACKBOARD_MAX_KEY_SIZE);
    *str_key = *key;

    return hashtable_insert(bb->table, str_key, value);
}

int blackboard_insert_pro(Blackboard *bb, const char *key, void *value, 
                          void (*freeval)(void *v))
{
    char *str_key = (char *)malloc(sizeof(char) * BLACKBOARD_MAX_KEY_SIZE);
    *str_key = *key;

    return hashtable_insert_pro(bb->table, str_key, value, free, freeval);
}

void *blackboard_search(Blackboard *bb, const char *key)
{
    return hashtable_search(bb->table, key);
}

int blackboard_remove(Blackboard *bb, const char *key)
{
    hashtable_remove(bb->table, key);

    return 0;
}

static size_t _blackboard_hash(void *path, unsigned int table_size)
{
	const char *str = (char *)path;

    unsigned long hash = 2166136261UL;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 16777619;
    }
	hash = hash % table_size;
	
    return hash;
}

static int _cmp_keys(void *a, void *b)
{
    const char *str_a = (char *)a;
    const char *str_b = (char *)b;

	return strcmp(str_a, str_b);	
}
