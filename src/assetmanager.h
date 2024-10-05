#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <raylib.h>

#define SPRITE_MAX_ASSET_PATH_LEN 50

typedef struct TextureAsset {
	char path[SPRITE_MAX_ASSET_PATH_LEN];
	Texture2D texture;
} TextureAsset;

void assetmanager_initialize();
void assetmanager_load_texture(const char *path);
TextureAsset *assetmanager_request_texture(const char *path);
void assetmanager_free_textureasset(const char *path);

#endif /* ASSETMANAGER_H */
