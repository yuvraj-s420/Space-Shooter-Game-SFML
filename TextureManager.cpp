#include "TextureManager.h"
#include <iostream>

const sf::Texture& TextureManager::getTexture(const std::string& filename) {
    // Add texture to the map if not already there, and return it
    if (textures.find(filename) == textures.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Failed to load texture: " << filename << std::endl;
        }
        textures[filename] = texture;
    }
    return textures[filename];
}

