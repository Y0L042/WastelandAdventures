/* Log:
 * @16:35 Wed, 02 Oct 2024
 *  Capitalizing all defines
 *  Adding free() callbacks
 */

#ifndef DRAY_H
#define DRAY_H

#include <stdlib.h>
#include <stdio.h>

#define DRAY_IS_EMPTY 0
#define DRAY_EMPTY_VALUE -1

/* No need for free() callback */
#define DRAY_INIT_VALUES(_dray, _type) \
        dray_init(_dray, sizeof(_type), NULL, DRAY_CAT_VALUE)

/* Need for free() callback */
#define DRAY_INIT_POINTERS(_dray, _pointer_type, _free_func) \
        dray_init(_dray, sizeof(_pointer_type *), _free_func, DRAY_CAT_POINTER) 

#define DRAY_DEFINE_EMPTY(_dray, _value, _type) \
        

#define DRAY_ADD_VALUE(_dray, _value, _type) \
    do { \
        _type _dray_val_add = (_type)(_value); \
        _dray_add_value(_dray, &_dray_val_add, sizeof(_type)); \
    } while (0)

#define DRAY_ADD_POINTER(_dray, _pointer) \
        _dray_add_pointer(_dray, _pointer) 

#define DRAY_GET_VALUE(_dray, _idx, _type) \
        (*((_type *)_dray_get_value(_dray, _idx, sizeof(_type))))

#define DRAY_POP_VALUE(_dray, _type) \
        (*((_type *)_dray_pop_value(_dray, sizeof(_type))))

#define DRAY_DEQUEUE_VALUE(_dray, _type) \
        (*((_type *)_dray_dequeue_value(_dray, sizeof(_type))))

#define DRAY_GET_POINTER(_dray, _idx, _type) \
        ((_type *)_dray_get_pointer(_dray, _idx))

#define DRAY_POP_POINTER(_dray, _type) \
        ((_type *)_dray_pop_pointer(_dray))

#define DRAY_DEQUEUE_POINTER(_dray, _type) \
        ((_type *)_dray_dequeue_pointer(_dray))



typedef enum DRAY_CAT {
    DRAY_CAT_VALUE = 0,
    DRAY_CAT_POINTER
} DRAY_CAT;

typedef struct DRay {
    void *data;
    size_t count;
    size_t capacity;
    size_t elem_size;
    int (*test_empty)(void *d);
    void (*free_data)(void *d);
    DRAY_CAT cat;
} DRay;

/* Initialize or reset DRay 
 */
void dray_init(DRay *dray, size_t elem_size, void (*free_func)(void *d), DRAY_CAT cat);

/* Frees all data of DRay (excluding the DRay itself). Use only with pointer
 * data 
 */
void dray_free_all_data(DRay *dray);

/* Manually set DRay size 
 */
void dray_set_size(DRay *dray, size_t size);

/* Set DRay empty value test
 */
void dray_set_empty_test(DRay *dray, int (*test_empty)(void *d));

/* Get pointer to element index
 */
void *dray_get_idx_ptr(DRay *dray, size_t idx);

/* Add data to end of DRay
 */
void dray_add_data(DRay *dray, void *data);

/* Inserts data at specified index. Overrides existing data
 */
void dray_insert_data(DRay *dray, void *data, size_t idx);

/* Frees/clears data at index, and resize DRay
 */
void dray_remove_idx(DRay *dray, size_t idx);

/* Frees/clears data at index, does not resize DRay
 */
void dray_clear_idx(DRay *dray, size_t idx);

/* Removes gaps in DRay
 */
void dray_defragment(DRay *dray);

/* Tests DRay index for empty value
 */
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
	if (idx >= dray->count) 
	{ 
		fprintf(stderr, 
          "Getting DRAY value, DRay idx out of bounds: idx: %d, dray size: %d\n", 
          idx, dray->count);
		return NULL; 
	}
    void *idx_ptr = dray_get_idx_ptr(dray, idx);
    if ( *((char *)idx_ptr) == 0)
    {
        return (char *)idx_ptr;
    }
    
    return idx_ptr;
}

static inline void *_dray_get_pointer(DRay *dray, int idx)
{
	if (idx >= dray->count) { return NULL; }
    void **idx_ptr_ptr = (void **)dray_get_idx_ptr(dray, idx);
    if ( *((char *)idx_ptr_ptr) == DRAY_EMPTY_VALUE)
    {
        return NULL;
    }

    return *idx_ptr_ptr;
}

static inline void *_dray_pop_value(DRay *dray, size_t type_size)
{
	if (dray->count == 0) { return NULL; }
	int idx = dray->count - 1;
	void *val = _dray_get_value(dray, idx, type_size);
    if ( *((char *)val) == DRAY_EMPTY_VALUE)
    {
        return (char *)val;
    }
	dray_remove_idx(dray, idx);

	return val;
}

static inline void *_dray_pop_pointer(DRay *dray)
{
	if (dray->count == 0) { return NULL; }
	int idx = dray->count - 1;
    void **idx_ptr_ptr = (void **)dray_get_idx_ptr(dray, idx);
    if ( *((char *)idx_ptr_ptr) == DRAY_EMPTY_VALUE)
    {
        return NULL;
    }
	dray_remove_idx(dray, idx);

    return *idx_ptr_ptr;
}

static inline void *_dray_dequeue_value(DRay *dray, size_t type_size)
{
	if (dray->count == 0) { return NULL; }
	int idx = 0;
	void *val = _dray_get_value(dray, idx, type_size);
    if ( *((char *)val) == DRAY_EMPTY_VALUE)
    {
        return (char *)val;
    }
	dray_remove_idx(dray, idx);

	return val;
}

static inline void *_dray_dequeue_pointer(DRay *dray)
{
	if (dray->count == 0) { return NULL; }
	int idx = 0;
    void **idx_ptr_ptr = (void **)dray_get_idx_ptr(dray, idx);
    if ( *((char *)idx_ptr_ptr) == DRAY_EMPTY_VALUE)
    {
        return NULL;
    }
	dray_remove_idx(dray, idx);

    return *idx_ptr_ptr;
}

#endif // DRAY_H
