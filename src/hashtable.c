#include "hashtable.h"

#include <stdlib.h>
#include <stdio.h>

#define HT_INIT_DELTA_IDX 4
#define HT_MAX_DELTA_IDX (sizeof(delta) / sizeof(unsigned short))

/**
 * the array of differences between a power-of-two and the largest prime less
 * than that power-of-two.
 */
unsigned short delta[] = { 0, 0, 1, 1, 3, 1, 3, 1, 5, 3, 3, 9, 3, 1, 3, 19, 15,
	1, 5, 1, 3, 9, 3, 15, 3, 39, 5, 39, 57, 3, 35, 1 };

static int get_size(HashTable *ht);
static int rehash(HashTable *ht);
static void insert_tail(HashTable *ht, HTEntry *entry, int hashidx);



HashTable *hashtable_create(
		float loadfactor,
		size_t (*hash)(void *, unsigned int),
		int (*cmp)(void *, void *)
	)
{
	HashTable *ht = (HashTable *)malloc(sizeof(HashTable));

	if (loadfactor <= 0.0f) {
		return NULL;
	}
	
	ht->delta_idx = HT_INIT_DELTA_IDX;
	int size = get_size(ht);
	ht->size = size;

	ht->table = (HTEntry **)calloc(size, sizeof(HTEntry));
	if (ht->table == NULL) {
		return NULL;
	}

	ht->entry_count = 0;
	ht->max_loadfactor = loadfactor;
	ht->hash = hash;
	ht->cmp = cmp;

	return ht;
}


int hashtable_insert(HashTable *ht, void *key, void *value)
{
	if (!(ht && key && value)) {
		return -1;
	}

	float current_loadfactor = (float)ht->entry_count / (float)ht->size;
	if (current_loadfactor > ht->max_loadfactor) {
		rehash(ht);
	}

	size_t hashidx = ht->hash(key, ht->size);
	if (hashidx >= ht->size) {
		printf("HashIdx %d larger than hashtable size %d!", hashidx, ht->size);
		return -2;
	}

	HTEntry *entry = ht->table[hashidx];
	while (entry != NULL) {
		if (ht->cmp(key, entry->key) == 0) {
			return -2;
		}
		entry = entry->next_ptr;
	}

	entry = (HTEntry *)malloc(sizeof(HTEntry));
	if (entry == NULL) {
		return -1;
	}

	entry->key = key;
	entry->value = value;

	insert_tail(ht, entry, hashidx);
	ht->entry_count++;

	return 0;
}

void *hashtable_search(HashTable *ht, void *key)
{
	if (!(ht && key)) {
		return NULL;
	}

	size_t hashidx = ht->hash(key, ht->size);
	HTEntry *entry;
	for (entry = ht->table[hashidx]; entry != NULL; entry = entry->next_ptr) {
		if (ht->cmp(key, entry->key) == 0) {
			return entry->value;
		}
	}

	return NULL;
}

int hashtable_free(HashTable *ht, void (*freekey)(void *k), void (*freeval)(void *v))
{
	if (!(ht && freekey && freeval)) {
		return -1;
	}

	HTEntry *p, *q;
	for (int i = 0; i < ht->size; i++) {
		p = ht->table[i];
		while (p != NULL) {
			q = p->next_ptr;
			if (p->key) {
				freekey(p->key);
			}
			if (p->value) {
				freeval(p->value);
			}
			free(p);
			p = q;
		}
	}

	free(ht->table);
	free(ht);

	return 0;
}

static int get_size(HashTable *ht)
{
	if (ht->delta_idx >= HT_MAX_DELTA_IDX) {
		return ht->size;
	}	

	return (1 << ht->delta_idx) - delta[ht->delta_idx];
}

static int rehash(HashTable *ht)
{
	unsigned int old_size = ht->size;
	HTEntry **old_table = ht->table;
	HTEntry *entry, *next_entry;
	
	if (ht->delta_idx+1 >= HT_MAX_DELTA_IDX) {
		return -1;
	}

	ht->delta_idx++;
	ht->size = get_size(ht);
	ht->table = (HTEntry **)calloc(ht->size, sizeof(HTEntry *));
	if (ht->table == NULL) {
		ht->delta_idx--;
		ht->size = old_size;
		ht->table = old_table;
	
		return -1;
	}

	ht->entry_count = 0;
	for (int i = 0; i < old_size; i++) {
		entry = old_table[i];
		while(entry) {
			next_entry = entry->next_ptr;
			int hashidx = ht->hash(entry->key, ht->size);
			insert_tail(ht, entry, hashidx);
			ht->entry_count++;
			entry = next_entry;
		}
	}

	return 0;
}

static void insert_tail(HashTable *ht, HTEntry *entry, int hashidx)
{
	HTEntry *current_entry;

	entry->next_ptr = NULL;
	if (ht->table[hashidx] == NULL) {
		ht->table[hashidx] = entry;
	} else {
		current_entry = ht->table[hashidx];
		while (current_entry->next_ptr != NULL) {
			current_entry = current_entry->next_ptr;
		}
		current_entry->next_ptr = entry;
	}
}

