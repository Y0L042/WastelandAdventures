#include "assetmanager.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "hashtable.h"

#define ASSETS_INIT (!pvt_assets_initialized ? assets_initialize() : (void)0 )

static int is_assetmanager_initialized = 0;
static HashTable *pvt_texture_asset_table;

static int cmp_textures(void *ta, void *tb);
static size_t hash_assets(void *t, unsigned int table_size);

void assetmanager_initialize()
{
	pvt_texture_asset_table = hashtable_create(
			10.0f,
			hash_assets,
			cmp_textures
		);

	is_assetmanager_initialized = 1;
}

void assetmanager_load_texture(const char *path)
{
	TextureAsset *ta = NULL;
	ta = (TextureAsset *)calloc(1, sizeof(TextureAsset));
	strcpy(ta->path, path);
	ta->path[SPRITE_MAX_ASSET_PATH_LEN-1] = '\0';
	ta->texture = LoadTexture(path);

	hashtable_insert(pvt_texture_asset_table, ta->path, ta);
}

TextureAsset *assetmanager_request_texture(const char *path)
{
	return (TextureAsset *)hashtable_search(pvt_texture_asset_table, path);

	return NULL;
}

void assetmanager_free_textureasset(const char *path)
{

}

static int cmp_textures(void *a, void *b)
{
	TextureAsset *ta = (TextureAsset *)a;
	TextureAsset *tb = (TextureAsset *)b;

	return strcmp(ta->path, tb->path);	
}

static size_t hash_assets(void *path, unsigned int table_size)
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

