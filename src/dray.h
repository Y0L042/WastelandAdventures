#ifndef DRAY_H
#define DRAY_H

#include "memory.h"

#define dray_init_values(_dray, _type) \
    ({ \
        size_t _type_size = sizeof(_type); \
        dray_init(_dray, _type_size); \
    })

#define dray_init_pointers(_dray, _pointer_type) \
    ({ \
        size_t _type_size = sizeof(_pointer_type *); \
        dray_init(_dray, _type_size); \
    })

#define dray_add_value(_dray, _value, _type) \
    ({ \
        _type _dray_val_add = _value; \
        dray_add_data(_dray, &_dray_val_add); \
    })

#define dray_add_pointer(_dray, _pointer) \
    ({ \
        dray_add_data(_dray, &_pointer); \
    })

#define dray_get_value(_dray, _idx, _type) \
    ({ \
        _type RETURN_dray_val_get; \
        _type *_dray_val_get_idx_ptr = (_type *)dray_get_idx_ptr(_dray, _idx); \
        RETURN_dray_val_get = *_dray_val_get_idx_ptr; \
        RETURN_dray_val_get; \
    })

#define dray_get_pointer(_dray, _idx, _type) \
    ({ \
        _type **_dray_ptr_ptr = (_type **)dray_get_idx_ptr(_dray, _idx); \
        _type *RETURN_dray_ptr = *_dray_ptr_ptr; \
        RETURN_dray_ptr; \
    })

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
// TODO Add a way to insert data, and move others up,
// i.e. you dont overwrite the data at that idx
void dray_remove_idx(DRay *dray, size_t idx);
void dray_clear_idx(DRay *dray, size_t idx);
void dray_defragment(DRay *dray);

// DRayIt dray_iter(DRay *dray);
#endif // DRAY_H
