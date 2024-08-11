#ifndef DRAY_H
#define DRAY_H

#include "memory.h"
#include "stdlib.h"

/* DRAY_EMPTY_VALUE must be less than 128 (it must fit inside a char) */
#define DRAY_EMPTY_VALUE 0b0101101
#define DRAY_EMPTY_POINTER NULL

#define dray_init_values(_dray, _type) \
        dray_init(_dray, sizeof(_type), DRAY_CAT_VALUE)

#define dray_init_pointers(_dray, _pointer_type) \
        dray_init(_dray, sizeof(_pointer_type *), DRAY_CAT_POINTER) 

#define dray_add_value(_dray, _value, _type) \
    do { \
        _type _dray_val_add = _value; \
        _dray_add_value(_dray, &_dray_val_add, sizeof(_type)); \
    } while (0)

#define dray_add_pointer(_dray, _pointer) \
        _dray_add_pointer(_dray, _pointer) 

#define dray_get_value(_dray, _idx, _type) \
        (*((_type *)_dray_get_value(_dray, _idx, sizeof(_type))))

#define dray_get_pointer(_dray, _idx, _type) \
        ((_type *)_dray_get_pointer(_dray, _idx))

typedef enum DRAY_CAT {
    DRAY_CAT_VALUE = 0,
    DRAY_CAT_POINTER
} DRAY_CAT;

typedef struct DRay {
    void *data;
    size_t count;
    size_t capacity;
    size_t elem_size;
    DRAY_CAT cat;
} DRay;

typedef struct DRayIter {
    DRay *dray;
} DRayIter;

void dray_init(DRay *dray, size_t elem_size, DRAY_CAT cat);
void dray_init_size(DRay *dray, size_t elem_size, size_t size, DRAY_CAT cat);
void dray_free(DRay *dray);
void dray_set_cat(DRay *dray, DRAY_CAT cat);
void *dray_get_idx_ptr(DRay *dray, size_t idx);
void dray_add_data(DRay *dray, void *data);
void dray_insert_data(DRay *dray, void *data, size_t idx);
// TODO Add a way to insert data, and move others up,
// i.e. you dont overwrite the data at that idx
void dray_remove_idx(DRay *dray, size_t idx);
void dray_clear_idx(DRay *dray, size_t idx);
void dray_defragment(DRay *dray);
int dray_test_for_empty_idx(DRay *dray, size_t idx);

static inline void _dray_add_value(DRay *dray, void *value, size_t value_size)
{
    dray_add_data(dray, value);
}

static inline void _dray_add_pointer(DRay *dray, void *pointer)
{
    dray_add_data(dray, &pointer);
}

static inline void *_dray_get_value(DRay *dray, int idx, size_t type_size)
{
    void *idx_ptr = dray_get_idx_ptr(dray, idx);

    if ( *((char *)idx_ptr) == DRAY_EMPTY_VALUE)
    {
        return (char *)idx_ptr;
    }
    
    return idx_ptr;
}

static inline void *_dray_get_pointer(DRay *dray, int idx)
{
    void **idx_ptr_ptr = (void **)dray_get_idx_ptr(dray, idx);

    if ( *((char *)idx_ptr_ptr) == DRAY_EMPTY_VALUE)
    {
        return NULL;
    }

    return *idx_ptr_ptr;
}

// DRayIt dray_iter(DRay *dray);
#endif // DRAY_H
