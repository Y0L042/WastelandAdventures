#include "dray.h"
#include "stdlib.h"
#include "stdio.h"

typedef struct TestStruct {
    int x;
    float y;
    int z;
    int k, l, m;
} TestStruct;

/* Function Prototypes */
void populate_dray_int(DRay *dray_int);
void print_dray_int(DRay *dray_int);

void populate_dray_ts(DRay *dray_ts);
void print_dray_ts(DRay *dray_ts);

void populate_dray_ptr(DRay *dray_ptr);
void print_dray_ptr(DRay *dray_ptr);

/* MAIN */
int main()
{
    printf("\n\nSize of TestStruct  =\t%d\n", sizeof(TestStruct));
    printf("Size of TestStruct* =\t%d\n\n", sizeof(TestStruct *));

    DRay dray_int; // DRay<int>
    dray_init(&dray_int, sizeof(int));
    populate_dray_int(&dray_int);
    print_dray_int(&dray_int);
    
    DRay dray_ts; // DRay<TestStruct>
    dray_init(&dray_ts, sizeof(TestStruct));
    populate_dray_ts(&dray_ts);
    print_dray_ts(&dray_ts);

    DRay dray_ptr; // DRay<TestStruct>
    dray_init(&dray_ptr, sizeof(TestStruct *));
    populate_dray_ptr(&dray_ptr);
    print_dray_ptr(&dray_ptr);

    return 0;
}

/* Function Implementations */
void populate_dray_int(DRay *dray_int)
{
    for (int i = 0; i < 100; i++)
    {
        //int x =  2 * i;
        //dray_add_data(dray_int, &x);
        dray_add_value(dray_int, 2 * i, int);
    }
}

void print_dray_int(DRay *dray_int)
{
    printf("\nPrinting DRay INT:\n");
    for (int i = 0; i < dray_int->count; i++)
    {
        //int *data = (int *)dray_get_idx_ptr(dray_int, i);
        //printf("idx %d \t=\t%d\n", i, *data);
        int data = dray_get_value(dray_int, i, int);
        printf("idx %d \t=\t%d\n", i, data);
    }
    printf("\nPrinting DRay INT Finished.\n");
}

void populate_dray_ts(DRay *dray_ts)
{
    for (int i = 0; i < 100; i++)
    {
        TestStruct ts = { i, (float)i * 2.5f, 1, 2, 3, 4 };
        dray_add_value(dray_ts, ts, TestStruct);
        // dray_add_data(dray_ts, &ts);
    }
}

void print_dray_ts(DRay *dray_ts)
{
    printf("\nPrinting DRay TS:\n");
    for (int i = 0; i < dray_ts->count; i++)
    {
        // TestStruct *data = (TestStruct *)dray_get_idx_ptr(dray_ts, i);
        // printf("idx %d \t=\t%d | %.2f | %d\n", i, data->x, data->y, data->z);
        TestStruct data = dray_get_value(dray_ts, i, TestStruct);
        printf("idx %d \t=\t%d | %.2f | %d\n", i, data.x, data.y, data.z);
    }
    printf("\nPrinting DRay TS Finished.\n");
}

void populate_dray_ptr(DRay *dray_ptr)
{
    for (int i = 0; i < 100; i++)
    {
        TestStruct *data = (TestStruct *)malloc(sizeof(TestStruct));
        data->x = 1; data->y = 2.0f; data->z = 5; data->k = 1; data->l = 2; data->m = 3;
        dray_add_pointer(dray_ptr, data);
    }
}

void print_dray_ptr(DRay *dray_ptr)
{
    printf("\nPrinting DRay PTR:\n");
    for (int i = 0; i < dray_ptr->count; i++)
    {
        // TestStruct **data_ptr = (TestStruct **)dray_get_idx_ptr(dray_ptr, i);
        // TestStruct *data =  *data_ptr;       
        TestStruct *data = dray_get_pointer(dray_ptr, i, TestStruct);
        printf("idx %d \t=\t%p\t%d | %.2f | %d\n", i, data, data->x, data->y, data->z);
    }
    printf("\nPrinting DRay PTR Finished.\n");
}






