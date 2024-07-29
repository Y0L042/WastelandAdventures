#include "cvec.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void cvec_int_init(CVecInt* cvec)
{
	cvec->count = 0;
	cvec->capacity = 0;
	cvec->data = NULL;
}

void cvec_int_free(CVecInt* cvec)
{
	FREE_ARRAY(int, cvec, cvec->capacity);
	cvec_int_init(cvec);
}

void cvec_int_add_item(CVecInt* cvec, int data)
{
	if (cvec->capacity  < cvec->count + 1)
	{
		int oldCapacity = cvec->capacity;
		cvec->capacity = GROW_CAPACITY(oldCapacity);
		cvec->data = GROW_ARRAY(int, cvec->data, oldCapacity, cvec->capacity);
	}

	cvec->data[cvec->count] = data;
	cvec->count++;
}

void cvec_int_remove_idx(CVecInt* cvec, int idx)
{
	if (idx >= cvec->count)
	{
		// Add some warning system?
		// printf("Out of bounds of valid Count!\n");
		return;
	}

	cvec->data[idx] = 0;

	// Takes O(N), but keeps item order
	int i;
	for (i = idx; i < cvec->count; i++)
	{
		cvec->data[i] = cvec->data[i+1]; 
	}

	cvec->count--;

	if (cvec->capacity > (cvec->count * 4))
	{
		int oldCapacity = cvec->capacity;
		cvec->capacity = SHRINK_CAPACITY(oldCapacity);
		cvec->data = GROW_ARRAY(int, cvec->data, oldCapacity, cvec->capacity);
	}
}

void cvec_int_remove_idx_noshrink(CVecInt* cvec, int idx)
{
	if (idx >= cvec->count)
	{
		// Add some warning system?
		// printf("Out of bounds of valid Count!\n");
		return;
	}

	cvec->data[idx] = 0;

	// Takes O(N), but keeps item order
	int i;
	for (i = idx; i < cvec->count; i++)
	{
		cvec->data[i] = cvec->data[i+1]; 
	}

	cvec->count--;
}

void cvec_int_clear_idx(CVecInt *cvec, int idx)
{
	if (idx < 0 || idx >= cvec->count)
	{
		printf("Index out of bounds: %d\n", idx);
		return;
	}
	cvec->data[idx] = INT_MIN;
}

void cvec_int_defragment(CVecInt *cvec)
{
	int count_sub = 0;
	int i = 0;
	while (i < cvec->count)
	{
		if (cvec->data[i] == INT_MIN)
		{
			cvec->data[i] = cvec->data[i+1];
			cvec->data[i+1] = INT_MIN;
			count_sub++;
		}
		else 
		{
			i++;
		}
	}

	cvec->count -= count_sub;
	if (cvec->count < 0) { printf("Defragment fault!"); }
}



void cvec_void_init(CVecVoid* cvec)
{
	cvec->count = 0;
	cvec->capacity = 0;
	cvec->data = NULL;
}

void cvec_void_free(CVecVoid* cvec)
{
	FREE_ARRAY(void*, cvec, cvec->capacity);
	cvec_void_init(cvec);
}

void cvec_void_add_item(CVecVoid* cvec, void* data)
{
	if (cvec->capacity  < cvec->count + 1)
	{
		int oldCapacity = cvec->capacity;
		cvec->capacity = GROW_CAPACITY(oldCapacity);
		cvec->data = GROW_ARRAY(void*, cvec->data, oldCapacity, cvec->capacity);
	}

	cvec->data[cvec->count] = data;
	cvec->count++;
}

void cvec_void_remove_idx(CVecVoid* cvec, int idx)
{
	if (idx >= cvec->count)
	{
		// Add some warning system?
		// printf("Out of bounds of valid Count!\n");
		return;
	}

	cvec->data[idx] = 0;

	// Takes O(N), but keeps item order
	int i;
	for (i = idx; i < cvec->count; i++)
	{
		cvec->data[i] = cvec->data[i+1]; 
	}

	cvec->count--;

	if (cvec->capacity > (cvec->count * 4))
	{
		int oldCapacity = cvec->capacity;
		cvec->capacity = SHRINK_CAPACITY(oldCapacity);
		cvec->data = GROW_ARRAY(void*, cvec->data, oldCapacity, cvec->capacity);
	}
}

void cvec_void_remove_idx_noshrink(CVecVoid* cvec, int idx)
{
	if (idx >= cvec->count)
	{
		// Add some warning system?
		// printf("Out of bounds of valid Count!\n");
		return;
	}

	cvec->data[idx] = 0;

	// Takes O(N), but keeps item order
	int i;
	for (i = idx; i < cvec->count; i++)
	{
		cvec->data[i] = cvec->data[i+1]; 
	}

	cvec->count--;
}

void cvec_void_clear_idx(CVecVoid *cvec, int idx)
{
	if (idx < 0 || idx >= cvec->count)
	{
		printf("Index out of bounds: %d\n", idx);
		return;
	}
	cvec->data[idx] = NULL;
}

void cvec_void_defragment(CVecVoid *cvec)
{
	int count_sub = 0;
	int i = 0;
	while (i < cvec->count)
	{
		if (cvec->data[i] == NULL)
		{
			cvec->data[i] = cvec->data[i+1];
			cvec->data[i+1] = NULL;
			count_sub++;
		}
		else 
		{
			i++;
		}
	}

	cvec->count -= count_sub;
	if (cvec->count < 0) { printf("Defragment fault!"); }
}



