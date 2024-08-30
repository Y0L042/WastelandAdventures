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
   	int _max = max + 1 - min; 
    int x = min + (rand() % (_max != 0 ? _max : 1));

    return x;
}

void binprintf(int v)
{
    unsigned int mask=1<<((sizeof(int)<<3)-1);
    while(mask) {
        printf("%d", (v&mask ? 1 : 0));
        mask >>= 1;
    }
	printf("\n");
}
