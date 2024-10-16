#include "../src/dray.h"
#include "stdlib.h"
#include "stdio.h"

typedef struct TestStruct {
    int x;
    float y;
    char z;
    int k, l, m;
} TestStruct;

/* Function Prototypes */
void populate_dray_int(DRay *dray_int);
void print_dray_int(DRay *dray_int);

void populate_dray_ts(DRay *dray_ts);
void print_dray_ts(DRay *dray_ts);

void populate_dray_ptr(DRay *dray_ptr);
void print_dray_ptr(DRay *dray_ptr);

static int LOOP_COUNT = 10;

/* MAIN */
int main()
{
    printf("\n\nSize of TestStruct  =\t%d\n", sizeof(TestStruct));
    printf("Size of TestStruct* =\t%d\n\n", sizeof(TestStruct *));

    DRay dray_int; // DRay<int>
    //dray_init(&dray_int, sizeof(int));
    dray_init_values(&dray_int, int);
    populate_dray_int(&dray_int);
    print_dray_int(&dray_int);
    
    dray_clear_idx(&dray_int, 4);
    print_dray_int(&dray_int);
    dray_remove_idx(&dray_int, 5);
    print_dray_int(&dray_int);

    dray_defragment(&dray_int);
    print_dray_int(&dray_int);
    
    DRay dray_ts; // DRay<TestStruct>
    //dray_init(&dray_ts, sizeof(TestStruct));
    dray_init_values(&dray_ts, TestStruct);
    populate_dray_ts(&dray_ts);
    print_dray_ts(&dray_ts);

    dray_remove_idx(&dray_ts, 0);
    print_dray_ts(&dray_ts);
    dray_clear_idx(&dray_ts, 6);
    print_dray_ts(&dray_ts);
    dray_remove_idx(&dray_ts, 8);
    print_dray_ts(&dray_ts);
    dray_defragment(&dray_ts);
    print_dray_ts(&dray_ts);

    DRay dray_ptr; // DRay<TestStruct *>
    //dray_init(&dray_ptr, sizeof(TestStruct *));
    dray_init_pointers(&dray_ptr, TestStruct);
    populate_dray_ptr(&dray_ptr);
    print_dray_ptr(&dray_ptr);

    dray_remove_idx(&dray_ptr, 0);
    print_dray_ptr(&dray_ptr);
    dray_clear_idx(&dray_ptr, 4);
    print_dray_ptr(&dray_ptr);
    dray_remove_idx(&dray_ptr, 8);
    print_dray_ptr(&dray_ptr);
    dray_defragment(&dray_ptr);
    print_dray_ptr(&dray_ptr);
    
    return 0;
}

/* Function Implementations */
void populate_dray_int(DRay *dray_int)
{
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        //int x =  2 * i;
        //dray_add_data(dray_int, &x);
        dray_add_value(dray_int, 2 * i, int);
    }
}

void print_dray_int(DRay *dray_int)
{
    printf("\nPrinting DRay INT:\n\n");
    for (int i = 0; i < dray_int->count; i++)
    {
        //int *data = (int *)dray_get_idx_ptr(dray_int, i);
        //printf("idx %d \t=\t%d\n", i, *data);
        int data = dray_get_value(dray_int, i, int);
        if (dray_test_for_empty_idx(dray_int, i)) 
        {
            printf("idx %d \t=\tEMPTY\n", i);
            continue; 
        }
        printf("idx %d \t=\t%d\n", i, data);
    }
    printf("\nPrinting DRay INT Finished.\n");
    printf("________________________________________________________________________________\n");
}

void populate_dray_ts(DRay *dray_ts)
{
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        TestStruct ts = { i, (float)i * 2.5f, 'a', 2, 3, 4 };
        dray_add_value(dray_ts, ts, TestStruct);
        // dray_add_data(dray_ts, &ts);
    }
}

void print_dray_ts(DRay *dray_ts)
{
    printf("\nPrinting DRay TS:\n\n");
    for (int i = 0; i < dray_ts->count; i++)
    {
        // TestStruct *data = (TestStruct *)dray_get_idx_ptr(dray_ts, i);
        // printf("idx %d \t=\t%d | %.2f | %d\n", i, data->x, data->y, data->z);
        TestStruct data = dray_get_value(dray_ts, i, TestStruct);
        if (dray_test_for_empty_idx(dray_ts, i)) 
        {
            printf("idx %d \t=\tEMPTY\n", i);
            continue; 
        }
        printf("idx %d \t=\t%d | %.2f | %c\n", i, data.x, data.y, data.z);
    }
    printf("\nPrinting DRay TS Finished.\n");
    printf("________________________________________________________________________________\n");
}

void populate_dray_ptr(DRay *dray_ptr)
{
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        TestStruct *data = (TestStruct *)malloc(sizeof(TestStruct));
        data->x = i; data->y = i * 2.5f; data->z = 'a'; data->k = 1; data->l = 2; data->m = 3;
        dray_add_pointer(dray_ptr, data);
    }
}

void print_dray_ptr(DRay *dray_ptr)
{
    printf("\nPrinting DRay PTR:\n\n");
    for (int i = 0; i < dray_ptr->count; i++)
    {
        // TestStruct **data_ptr = (TestStruct **)dray_get_idx_ptr(dray_ptr, i);
        // TestStruct *data =  *data_ptr;       
        TestStruct *data = dray_get_pointer(dray_ptr, i, TestStruct);
        if (dray_test_for_empty_idx(dray_ptr, i)) 
        {
            printf("idx %d \t=\tEMPTY\n", i);
            continue; 
        }
        printf("idx %d \t=\t%p\t%d | %.2f | %c\n", i, data, data->x, data->y, data->z);
    }
    printf("\nPrinting DRay PTR Finished.\n");
    printf("________________________________________________________________________________\n");
}






