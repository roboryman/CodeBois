#include "TextureManager.h"

/*
*
*                   TextureManager.cpp
*   -------------------------------------------------------
*   Some code used from a team member's MineSweeper project.
*
*/

unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string key) {
    textures[key].loadFromFile(key);
}

sf::Texture& TextureManager::GetTexture(string key) {
    if(textures.find(key) == textures.end()) {
        LoadTexture(key);
    }
    return textures[key];
}

void TextureManager::ClearTexture(string key) {
    textures.erase(key);
}

void TextureManager::ClearTextures() {
    textures.clear();
}