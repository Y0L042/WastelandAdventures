#ifndef DRAY_H
#define DRAY_H

#include "memory.h"

typedef struct DRay {
    void *data;
    size_t count;
    size_t capacity;
    size_t elem_size;
} DRay;

typedef struct DRayIter {
    DRay *dray;
} DRayIter;

void dray_init(DRay *dray, size_t elem_size);
void dray_init_size(DRay *dray, size_t elem_size, size_t size);
void dray_free(DRay *dray);
void *dray_get_idx_ptr(DRay *dray, size_t idx);
void dray_add_data(DRay *dray, void *data);
void dray_insert_data(DRay *dray, void *data, size_t idx);
void dray_remove_idx(DRay *dray, size_t idx);
void dray_clear_idx(DRay *dray, size_t idx);
void dray_defragment(DRay *dray);

// DRayIt dray_iter(DRay *dray);
#endif // DRAY_H
