#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <raylib.h>
#include "uuid.h"

#define SPRITE_MAX_ASSET_PATH_LEN 50

typedef UUID AssetUUID;

typedef struct TextureAsset {
    UUID uuid;
	char path[SPRITE_MAX_ASSET_PATH_LEN];
	Texture2D texture;
} TextureAsset;

void assetmanager_initialize();
void assetmanager_load_texture(const char *path);
TextureAsset *assetmanager_request_textureasset(UUID uuid);
UUID assetmanager_request_textureasset_uuid(const char *path);
void assetmanager_free_textureasset(UUID uuid);

#endif /* ASSETMANAGER_H */
