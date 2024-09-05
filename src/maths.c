#include "maths.h"
#include "math.h"
#include "stdlib.h"
#include "time.h"

#define FLT_EPSILON 0.0001f
#define DBL_EPSILON 0.00001

static int _maths_initialized = 0;

void _maths_initialize()
{
    srand(time(NULL));
    _maths_initialized = 1;
}

int maths_cmpf(float a, float b)
{
	int result = (fabs(a - b) < (FLT_EPSILON * fabs(a + b)));

	return result;
}

int maths_cmpd(double a, double b)
{
	int result = (fabs(a - b) < (DBL_EPSILON * fabs(a + b)));

	return result;
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
