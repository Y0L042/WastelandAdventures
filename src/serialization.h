#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <flecs.h>

typedef enum SerializationCodes {
    SERIALIZATION_SUCCESS = 0,
    SERIALIZATION_ENTITY_TO_JSON_FAILED,
    SERIALIZATION_ENTITY_FROM_JSON_FAILED
} SerializationCodes;

char *read_chars_from_file(const char *path);
int write_chars_to_file(char *chars, const char *path);


int serialize_entity_to_json_file(ecs_world_t *world, ecs_entity_t entity, const char *path);

ecs_entity_t deserialize_entity_from_json(ecs_world_t *world, const char *json_chars);

/* --- Loading Entity Definitions --- */
void DEP_load_entity_definitions_from_file(ecs_world_t *world, const char *path);


/* --- Loading/Saving Game State --- */
void deserialize_entities_from_json_file(ecs_world_t *world, const char *path);

#endif /* SERIALIZATION_H */
