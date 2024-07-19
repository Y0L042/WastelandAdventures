#include "uuid.h"

#include <time.h>
#include <stdlib.h>
	
uint8_t uuid_initialized = 0;
unsigned long long uuid_seed = 0;

UUID uuid_generate()
{
	if (!uuid_initialized)
	{
		uuid_init();
	}

	return genrand64_int64();
}

void uuid_init()
{
	if (uuid_seed == 0)
	{
		time_t t;
		srand((unsigned) time(&t));
		init_genrand64(rand());
	}
	else
	{
		init_genrand64(uuid_seed);
	}
	uuid_initialized = 1;
}

void uuid_set_seed(unsigned long long seed)
{
	uuid_seed = seed;
}
