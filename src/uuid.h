#ifndef UUID_H
#define UUID_H

#include <stdint.h>

#define UUID uint64_t

extern void init_genrand64(unsigned long long seed);
extern unsigned long long genrand64_int64(void);

UUID uuid_generate();
void uuid_init();
void uuid_set_seed(unsigned long long seed);


#endif // UUID_H
