#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


class TextureManager {
public:
    const sf::Texture& getTexture(const std::string &filename);

private:
    std::unordered_map<std::string, sf::Texture> textures;
};
