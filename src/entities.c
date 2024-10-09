#include "entities.h"

#include <string.h>

#include <raylib.h>
#include <flecs.h>
#include <cJSON.h>
#include "log.h"

#include "hashtable.h"
#include "serialization.h"



/* --- Entity Definitions Structure --- */
#define ENTITY_DEFS_JSON "entity_defs"
    #define META_JSON "meta"
    /* meta data, like entity key_name... */
        #define KEY_NAME_JSON "key_name"
    #define ECS_DATA_JSON "ecs_data"
    /* ecs data, like components... */

/* ------------------------------------ */



/* --- Variables --- */
static HashTable *entity_table;

/* --- Private Function Prototypes --- */
static size_t hash(void *key, unsigned int size);
static int cmp(void *key_a, void *key_b);
/* ----------------------------------- */



void entity_table_initialize()
{
    entity_table = hashtable_create(0.75f, hash, cmp);
}

void entity_table_free()
{
    hashtable_free(entity_table);
}



EntityErrorCodes load_entity_definitions_from_file(const char *path) 
{
    /* read json from file */
    char *file_json_str = read_chars_from_file(path);
    cJSON *file_json = cJSON_Parse(file_json_str);
    free(file_json_str);

    if (file_json == NULL) {
        log_error("Error parsing JSON file!");
        return ENTITY_LOADING_FAILED;
    }

    /* array of entities */
    cJSON *entities_json = cJSON_GetObjectItem(file_json, ENTITY_DEFS_JSON);
    if (!cJSON_IsArray(entities_json)) {
        log_error("Entities not an array!");
        cJSON_Delete(file_json);
        return ENTITY_LOADING_FAILED;
    }

    int num_entities = cJSON_GetArraySize(entities_json);
    for (int i = 0; i < num_entities; i++) {

        /* get current entity */
        cJSON *entity_json = cJSON_GetArrayItem(entities_json, i);
        if (entity_json == NULL) {
            log_error("Error parsing entity from JSON array!");
            continue;
        }

        /* extract "meta" from current entity */
        cJSON *meta_json = cJSON_GetObjectItem(entity_json, META_JSON);
        if (!meta_json) {
            log_error("ecs_data not found in entity!");
            continue;
        }
        
        /* extract "key_name" from "meta" */
        cJSON *key_name_json = cJSON_GetObjectItem(meta_json, KEY_NAME_JSON);
        if (!key_name_json) {
            log_error("key_name_json not found in meta!");
            continue;
        }

        /* extract "ecs_data" from current entity */
        cJSON *ecs_data_json = cJSON_GetObjectItem(entity_json, ECS_DATA_JSON);
        if (!ecs_data_json) {
            log_error("ecs_data not found in entity!");
            continue;
        }

        /* Add entity data to hashtable */
        static int i = 0;
        char *key_name_str = cJSON_GetStringValue(key_name_json);
        char *ecs_data_json_str = cJSON_Print(ecs_data_json);
        log_debug("Entity table entry %d:\n{ %s : %s }", ++i, key_name_str, ecs_data_json_str);
        hashtable_insert(entity_table, key_name_str, ecs_data_json_str);
    }

    cJSON_Delete(file_json);

    return ENTITY_LOADED_SUCCESSFULLY;
}



ecs_entity_t create_entity_from_table(ecs_world_t *world, char *key_name)
{
    char *entity_json = (char *)hashtable_search(entity_table, key_name);
    if (entity_json == NULL) {
        return ENTITY_NOT_FOUND_IN_TABLE;
    }
    ecs_entity_t entity = deserialize_entity_from_json(world, entity_json);
    if (!ecs_is_alive(world, entity)) {
        return ENTITY_NOT_CREATED;
    }

    return entity;
}



void print_entity_error(EntityErrorCodes code)
{
    switch (code) {
        case ENTITY_LOADED_SUCCESSFULLY:
            log_debug("Entity loaded successfully!");
            break;
        case ENTITY_LOADING_FAILED:
            log_error("Entity loading failed!");
            break;
        case ENTITY_NOT_FOUND_IN_TABLE:
            log_error("Entity not found in table!");
            break;
        case ENTITY_NOT_CREATED:
            log_error("ECS Entity not created!");
            break;
        default:
            log_error("Unknown entity error!");
            break;
    }
}



static size_t hash(void *key, unsigned int size)
{
	const char *str = (char *)key;
    unsigned long hash = 2166136261UL;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 16777619;
    }
	hash = hash % size;
	

    return hash;
}

static int cmp(void *key_a, void *key_b)
{
    return strcmp((char *)key_a, (char *)key_b);
}
