#include "dray.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"

#define DRAY_DATA_PTR_AT_IDX(_dray, _i) \
    ((void *)((char *)dray->data + (_i * dray->elem_size)))

/*
 * Private Function Declarations
 */
static void dray_set_cat(DRay *dray, DRAY_CAT cat);
static int _default_test_empty(void *d);

/* 
 * */

void dray_init(DRay *dray, size_t elem_size, void (*free_func)(void *d), DRAY_CAT cat)
{
    dray->count = 0;
    dray->capacity = 0;
    dray->elem_size = elem_size;
    dray->data = NULL;
    dray->test_empty = _default_test_empty;
    dray->free_data = free_func;
    dray_set_cat(dray, cat);
}

void dray_free_all_data(DRay *dray)
{
    switch (dray->cat) {
        case DRAY_CAT_VALUE:
            FREE_ARRAY_SIZE(dray->elem_size, dray->data, dray->capacity);
            break;
        case DRAY_CAT_POINTER:
            if (dray->free_data)
            {
                for (int i = 0; i < dray->count; i++)
                {
                    dray->free_data(DRAY_DATA_PTR_AT_IDX(dray, i));
                }
            }
            break;
        default:
            break;
    }
    dray_init(dray, dray->elem_size, dray->free_data, dray->cat);
}

void dray_set_size(DRay *dray, size_t size)
{
    dray->capacity = size;
    dray->data = GROW_ARRAY_SIZE(dray->elem_size, dray->data, 0, dray->capacity);
    if (dray->cat == DRAY_CAT_VALUE)
    {
        memset(dray->data, 0, dray->capacity * dray->elem_size);
    } 
    else if (dray->cat == DRAY_CAT_POINTER)
    {
        memset(dray->data, 0, dray->capacity * dray->elem_size);
    }
}

void dray_set_empty_test(DRay *dray, int (*test_empty)(void *d))
{
    if (test_empty)
    {
        dray->test_empty =  test_empty;
    }
}

void *dray_get_idx_ptr(DRay *dray, size_t idx)
{
    void *idx_ptr = (char *)dray->data + (idx * dray->elem_size);

    return idx_ptr;
}

void dray_add_data(DRay *dray, void *data)
{
    if (dray->capacity < (dray->count + 1))
    {
        int old_capacity = dray->capacity;
        dray->capacity = GROW_CAPACITY(old_capacity);
        dray->data = GROW_ARRAY_SIZE(
                dray->elem_size, 
                dray->data, 
                old_capacity,
                dray->capacity
            );
    }

    void *idx_ptr = (char *)dray->data + (dray->count * dray->elem_size); 
    memcpy(idx_ptr, data, dray->elem_size);  
    dray->count++;
}

void dray_insert_data(DRay *dray, void *data, size_t idx)
{
    /* Resize array capacity */
    if (dray->capacity < (idx + 1))
    {
        int old_capacity = dray->capacity;
        dray->capacity = idx + 1;
        dray->data = GROW_ARRAY_SIZE(
                dray->elem_size, 
                dray->data, 
                old_capacity,
                dray->capacity
            );
    }

    /* Fill gap with 0 if index is larger than current count; update count */
    void *idx_ptr;
    if (dray->count < idx + 1)
    {
        idx_ptr = (char *)dray->data + (dray->count * dray->elem_size);
        size_t gap_size = (idx + 1 - dray->count) * dray->elem_size;
        if (dray->cat == DRAY_CAT_VALUE)
        {
            memset(idx_ptr, 0, gap_size);
        } 
        else if (dray->cat == DRAY_CAT_POINTER)
        {
            /* Free data if necessary */
            if (dray->cat == DRAY_CAT_POINTER && dray->free_data)
            {
                dray->free_data(DRAY_DATA_PTR_AT_IDX(dray, idx));
            }
            memset(idx_ptr, 0, gap_size);
        }
        dray->count = idx + 1;
    }

    idx_ptr = (char *)dray->data + (idx * dray->elem_size);
    memcpy(idx_ptr, data, dray->elem_size);
}

void dray_remove_idx(DRay *dray, size_t idx)
{
    if (idx > dray->count - 1)
    {
        /* Add out-of-bounds error */
        return;
    }

    /* Free data if necessary */
    if (dray->cat == DRAY_CAT_POINTER && dray->free_data)
    {
        dray->free_data(DRAY_DATA_PTR_AT_IDX(dray, idx));
    }

    /* shift elements after idx by one position */
    if (dray->count > 1 && idx < (dray->count - 1))
    {
        void *source_ptr = (char *)dray->data + ((idx + 1) * dray->elem_size);
        void *target_ptr = (char *)dray->data + (idx * dray->elem_size);
        size_t move_size = (dray->count - (idx + 1)) * dray->elem_size;
        memmove(target_ptr, source_ptr, move_size);  
    }

    dray->count--;

    /* Resize array if necessary */
    if (dray->capacity > (dray->count * 4))
    {
        int old_capacity = dray->capacity;
        dray->capacity = SHRINK_CAPACITY(old_capacity);
        dray->data = GROW_ARRAY_SIZE(
                dray->elem_size,
                dray->data,
                old_capacity,
                dray->capacity
            );
    }
}

void dray_clear_idx(DRay *dray, size_t idx)
{
    if (idx > dray->count - 1)
    {
        /* Add out-of-bounds error */
        return;
    }
     
    void *idx_ptr = (char *)dray->data + (idx * dray->elem_size);
    if (dray->cat == DRAY_CAT_VALUE)
    {
        //memset(idx_ptr, DRAY_EMPTY_VALUE, dray->elem_size);    
        memset(idx_ptr, 0, dray->elem_size);    
    } 
    else if (dray->cat == DRAY_CAT_POINTER)
    {
        /* Free data if necessary */
        if (dray->cat == DRAY_CAT_POINTER && dray->free_data)
        {
            dray->free_data(DRAY_DATA_PTR_AT_IDX(dray, idx));
        }
        memset(idx_ptr, 0, dray->elem_size);    
    }
}

void dray_defragment(DRay *dray)
{
    for (int i = 0; i < dray->count; i++)
    {
        if (dray_test_for_empty_idx(dray, i))
        {
            dray_remove_idx(dray, i--);
        }
    }
}

int dray_test_for_empty_idx(DRay *dray, size_t idx)
{
    switch (dray->cat) {
        case DRAY_CAT_VALUE:
            return dray->test_empty(&dray->data[idx]);
            break;
        case DRAY_CAT_POINTER:
           return DRAY_DATA_PTR_AT_IDX(dray, idx) != NULL; 
    }

    return !DRAY_IS_EMPTY;
}

static void dray_set_cat(DRay *dray, DRAY_CAT cat)
{
    dray->cat = cat;
}

static int _default_test_empty(void *d)
{
    if (d == NULL) 
    {
        return DRAY_IS_EMPTY;
    }

    return !DRAY_IS_EMPTY;
}

