#include "serialization.h"

#include <stdio.h>

#include <cJSON.h>
#include <cJSON_Utils.h>

#include "log.h"

#define MAX_FILE_CHAR_COUNT 1000000 /* 1 million chars */

char *read_chars_from_file(const char *path)
{
    FILE *file = fopen(path, "r");

    char *buffer = (char *)calloc(MAX_FILE_CHAR_COUNT, sizeof(char));
    int i = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
          buffer[i] = c;
          i++;
    }
    buffer[i] = '\0';
    buffer = (char *)realloc(buffer, (sizeof(char) * i) + 8); /* resize buffer */

    fclose(file);

    return buffer;
}

int write_chars_to_file(char *chars, const char *path)
{
    FILE *file = fopen(path, "w");

    int results = fputs(chars, file);
    if (results == EOF) {
       return 1; 
    }
    fclose(file);

    return 0;
}

int serialize_entity_to_json_file(ecs_world_t *world, ecs_entity_t entity, const char *path)
{
    log_info("ENTITY_TO_JSON_FILE START\n");

    ecs_entity_to_json_desc_t params = ECS_ENTITY_TO_JSON_INIT;
    char *json_entity = ecs_entity_to_json(world, entity, NULL);

    write_chars_to_file(json_entity, path);

    log_info("%s", json_entity);

    ecs_os_free(json_entity);

    log_info("ENTITY_TO_JSON_FILE END\n");

    return SERIALIZATION_SUCCESS;
}

ecs_entity_t deserialize_entity_from_json_file(ecs_world_t *world, const char *path)
{
    log_info("ENTITY_FROM_JSON_FILE START\n");

    char *json_chars = read_chars_from_file(path);
    if (json_chars == NULL) {
        return SERIALIZATION_ENTITY_FROM_JSON_FAILED;
    }
    log_info("JSON_CHARS:\n%s", json_chars);

    ecs_entity_t e = ecs_new(world);
    ecs_entity_from_json(world, e, json_chars, NULL);

    free(json_chars);

    log_info("ENTITY_FROM_JSON_FILE END\n");

    return e;
}



