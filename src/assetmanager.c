#include "assetmanager.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "hashtable.h"

#define ASSETS_INIT (!pvt_assets_initialized ? assets_initialize() : (void)0 )

static int is_assetmanager_initialized = 0;

static HashTable *pvt_texture_asset_table;
static int cmp_textures(void *ta, void *tb);
static size_t hash_assets(void *uuid, unsigned int table_size);

static HashTable *pvt_path_to_uuid_table;
static int cmp_uuids(void *uuid_a, void *uuid_b);
static size_t hash_paths(void *path, unsigned int table_size);

void assetmanager_initialize()
{
	pvt_texture_asset_table = hashtable_create(
			10.0f,
			hash_assets,
			cmp_textures
		);
	pvt_path_to_uuid_table = hashtable_create(
			10.0f,
			hash_paths,
			cmp_uuids
		);

	is_assetmanager_initialized = 1;
}

void assetmanager_load_texture(const char *path)
{
	TextureAsset *ta = NULL;
	ta = (TextureAsset *)calloc(1, sizeof(TextureAsset));
	strcpy(ta->path, path);
    ta->uuid = NEW_UUID;
	ta->path[SPRITE_MAX_ASSET_PATH_LEN-1] = '\0';
	ta->texture = LoadTexture(path);

	hashtable_insert(pvt_texture_asset_table, &ta->uuid, ta);
    hashtable_insert(pvt_path_to_uuid_table, &ta->path, &ta->uuid);
}

TextureAsset *assetmanager_request_textureasset(UUID uuid)
{
	return (TextureAsset *)hashtable_search(pvt_texture_asset_table, &uuid);
}

UUID assetmanager_request_textureasset_uuid(const char *path)
{
    UUID *uuid = (UUID *)hashtable_search(pvt_path_to_uuid_table, &path);
    if (uuid == NULL) {
        return UUID_NULL;
    }

    return *uuid;
}

void assetmanager_free_textureasset(UUID uuid)
{

}

static int cmp_textures(void *a, void *b)
{
	TextureAsset *ta = (TextureAsset *)a;
	TextureAsset *tb = (TextureAsset *)b;

	return uuid_equals(ta->uuid, tb->uuid);	
}

static size_t hash_assets(void *uuid_, unsigned int table_size)
{
	const UUID *uuid = (UUID *)uuid_;
    uint64_t hash = 1469598103934665603ULL;
    for (int i = 0; i < 16; ++i) {
        hash ^= uuid->bytes[i];    // XOR the hash with the current byte
        hash *= 1099511628211ULL;   // Multiply by the FNV prime
    }
	hash = hash % table_size;

    return hash;
}

static int cmp_uuids(void *uuid_a_, void *uuid_b_)
{
    UUID *uuid_a = (UUID *)uuid_a_;
    UUID *uuid_b = (UUID *)uuid_b_;

    return uuid_equals(*uuid_a, *uuid_b);
}

static size_t hash_paths(void *path, unsigned int table_size)
{
	const char *str = (char *)path;
    unsigned long hash = 2166136261UL;
    while (*str) {
        hash ^= (unsigned char)*str++;
        hash *= 16777619;
    }
	hash = hash % table_size;

    return hash;
}

