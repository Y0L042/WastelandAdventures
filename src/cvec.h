#ifndef CVEC_H
#define CVEC_H

#include "memory.h"

typedef struct {
	int count;
	int capacity;
	int *data;
} CVecInt;

void cvec_int_init(CVecInt *cvec);
void cvec_int_free(CVecInt *cvec);
void cvec_int_add_item(CVecInt *cvec, int data);
void cvec_int_remove_idx(CVecInt *cvec, int idx);
void cvec_int_remove_idx_noshrink(CVecInt *cvec, int idx);
void cvec_int_clear_idx(CVecInt *cvec, int idx);
void cvec_int_defragment(CVecInt *cvec);

#define cvec_void_get_item(cvecvoid, idx, T)\
	(T *)(cvecvoid.data[idx])

typedef struct {
	int count;
	int capacity;
	void **data;
} CVecVoid;

void cvec_void_init(CVecVoid *cvec);
void cvec_void_free(CVecVoid *cvec);
void cvec_void_add_item(CVecVoid *cvec, void *data);
void cvec_void_remove_idx(CVecVoid *cvec, int idx);
void cvec_void_remove_idx_noshrink(CVecVoid *cvec, int idx);
void cvec_void_clear_idx(CVecVoid *cvec, int idx);
void cvec_void_defragment(CVecVoid *cvec);
#endif
