#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "hashtable.h"

#define BLACKBOARD_MAX_KEY_SIZE 64

#define BLACKBOARD_SEARCH_VALUE(_bb, _key, _T) \
	(*(_T *)blackboard_search(_bb, _key))

#define BLACKBOARD_SEARCH_POINTER(_bb, _key, _T) \
	((_T *)blackboard_search(_bb, _key))

typedef struct Blackboard {
   HashTable *table; 
} Blackboard;

Blackboard *blackboard_create();
void blackboard_free(Blackboard *bb);
int blackboard_insert(Blackboard *bb, const char *key, void *value);
int blackboard_insert_pro(Blackboard *bb, const char *key, void *value, 
                          void (*freeval)(void *v));
int blackboard_remove(Blackboard *bb, const char *key);
void *blackboard_search(Blackboard *bb, const char *key);

#endif // BLACKBOARD_H
