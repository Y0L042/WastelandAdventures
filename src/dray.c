#include "dray.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void dray_init(DRay *dray, size_t elem_size, DRAY_CAT cat)
{
    dray->count = 0;
    dray->capacity = 0;
    dray->elem_size = elem_size;
    dray->data = NULL;
    dray_set_cat(dray, cat);
}

void dray_init_size(DRay *dray, size_t elem_size, size_t size, DRAY_CAT cat)
{
    dray->count = 0;
    dray->capacity = size;
    dray->elem_size = elem_size;
    dray->data = GROW_ARRAY_SIZE(dray->elem_size, dray->data, 0, dray->capacity);
    dray_set_cat(dray, cat);
    if (dray->cat == DRAY_CAT_VALUE)
    {
        memset(dray->data, DRAY_EMPTY_VALUE, dray->capacity * dray->elem_size);
    } 
    else if (dray->cat == DRAY_CAT_POINTER)
    {
        memset(dray->data, DRAY_EMPTY_VALUE, dray->capacity * dray->elem_size);
    }
}

void dray_free(DRay *dray)
{
    FREE_ARRAY_SIZE(dray->elem_size, dray->data, dray->capacity);
    dray_init(dray, dray->elem_size, dray->cat);
}

void dray_set_cat(DRay *dray, DRAY_CAT cat)
{
    dray->cat = cat;
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
            memset(idx_ptr, DRAY_EMPTY_VALUE, gap_size);
        } 
        else if (dray->cat == DRAY_CAT_POINTER)
        {
            memset(idx_ptr, DRAY_EMPTY_VALUE, gap_size);
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
        memset(idx_ptr, DRAY_EMPTY_VALUE, dray->elem_size);    
    } 
    else if (dray->cat == DRAY_CAT_POINTER)
    {
        memset(idx_ptr, DRAY_EMPTY_VALUE, dray->elem_size);    
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
    int test_empty = (int)dray_get_value(dray, idx, char);
    if (test_empty == DRAY_EMPTY_VALUE)
    {
        return 1;
    }

    return 0;
}

