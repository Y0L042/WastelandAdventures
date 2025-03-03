#ifndef UUID_H
#define UUID_H

#include <stdint.h>
#include <string.h>

// #define IS_UUID_VALID(_uuid) \
//     (memcmp(_uuid.bytes, UUID_NULL.bytes, 16) != 0)
//
// #define UUID_EQUALS(_uuidA, _uuidB) \
//     (memcmp(_uuidA.bytes, _uuidB.bytes, 16) == 0)

#define NEW_UUID \
    (uuid_create_random())

#define UUID_NULL \
    ((UUID){ { 0 } })



/* 128 bit UUID */
typedef struct UUID {
    uint8_t bytes[16];
} UUID;


int uuid_equals(UUID uuid_a, UUID uuid_b);
int is_uuid_valid(UUID uuid);
UUID uuid_create_random();

#endif /* UUID_H */
