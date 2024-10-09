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

ecs_entity_t deserialize_entity_from_json(ecs_world_t *world, const char *json_chars)
{
    log_info("ENTITY_FROM_JSON_FILE START\n");

    if (json_chars == NULL) {
        return SERIALIZATION_ENTITY_FROM_JSON_FAILED;
    }
    log_info("JSON_CHARS:\n%s", json_chars);

    ecs_entity_t e = ecs_new(world);
    ecs_entity_from_json(world, e, json_chars, NULL);

    log_info("ENTITY_FROM_JSON_FILE END\n");

    return e;
}



void DEP_load_entity_definitions_from_file(ecs_world_t *world, const char *path)
{
    /* read json from file */
    char *file_json_str = read_chars_from_file(path);
    log_info("json_chars:\n%s\n", file_json_str);
    cJSON *file_json = cJSON_Parse(file_json_str);
    free(file_json_str);

    if (file_json == NULL) {
        log_error("Error parsing JSON file!");
        return;
    }

    /* array of entities */
    cJSON *entities_json = cJSON_GetObjectItem(file_json, "entities");
    if (!cJSON_IsArray(entities_json)) {
        log_error("Entities not an array!");
        cJSON_Delete(file_json);
        return;
    }

    int num_entities = cJSON_GetArraySize(entities_json);
    for (int i = 0; i < num_entities; i++) {

        /* get current entity */
        cJSON *entity_json = cJSON_GetArrayItem(entities_json, i);
        if (entity_json == NULL) {
            log_error("Error parsing entity from JSON array!");
            continue;
        }

        /* extract "ecs_data" from current entity */
        cJSON *ecs_data_json = cJSON_GetObjectItem(entity_json, "ecs_data");
        if (!ecs_data_json) {
            log_error("ecs_data not found in entity!");
            continue;
        }

        char *ecs_data_json_str = cJSON_Print(ecs_data_json);
        log_info("ecs_data JSON:\n%s\n", ecs_data_json_str);

        deserialize_entity_from_json(world, ecs_data_json_str);

        free(ecs_data_json_str);
    }

    cJSON_Delete(file_json);
}



void deserialize_entities_from_json_file(ecs_world_t *world, const char *path)
{
    char *json_chars = read_chars_from_file(path);
    log_info("json_chars:\n%s\n", json_chars);
    cJSON *json = cJSON_Parse(json_chars);
    if (json == NULL) {
        log_error("Error parsing JSON!");
        return;
    }

    cJSON *entities = cJSON_GetObjectItem(json, "entities");
    if (!cJSON_IsArray(entities)) {
        log_error("Entities not an array!");
        cJSON_Delete(json);
        return;
    }

    int num_entities = cJSON_GetArraySize(entities);
    for (int i = 0; i < num_entities; i++) {
        cJSON *entity = cJSON_GetArrayItem(entities, i);
        char *entity_str = cJSON_Print(entity);
        log_info("Entity JSON:\n%s\n", entity_str);
        deserialize_entity_from_json(world, entity_str);
        free(entity_str);
    }
}
