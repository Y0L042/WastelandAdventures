#ifndef ENTITIES_H
#define ENTITIES_H

#include <flecs.h>

typedef enum EntityErrorCodes {
    ENTITY_LOADED_SUCCESSFULLY = 0,
    ENTITY_LOADING_FAILED,
    ENTITY_NOT_FOUND_IN_TABLE,
    ENTITY_NOT_CREATED
} EntityErrorCodes;

void entity_table_initialize();
EntityErrorCodes load_entity_definitions_from_file(const char *path);
ecs_entity_t create_entity_from_table(ecs_world_t *world, char *key_name);

void print_entity_error(EntityErrorCodes code);

#endif /* ENTITIES_H */
