#include "maths.h"
#include "stdlib.h"
#include "time.h"

static int _maths_initialized = 0;

void _maths_initialize()
{
    srand(time(NULL));
    _maths_initialized = 1;
}

int maths_randbetween_int(int min, int max)
{
    if (!_maths_initialized) { _maths_initialize(); }
    
    int x = min + (rand() % (max + 1 - min));

    return x;
}
