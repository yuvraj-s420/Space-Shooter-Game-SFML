#include "Background.h"
#include <iostream>

void Background::initialize()
{

    sf::Vector2u sizeU = texture.getSize();
    sf::Vector2f center(sizeU.x / 2.f, sizeU.y / 2.f);  // vector2f center cooridnate because setOrigin requires it
    sf::Vector2f scale_factors(window_width / sizeU.x, window_height / sizeU.y);
    
    sprite.setScale(scale_factors);
    sprite.setOrigin(center);
    sprite.setPosition({window_width / 2.f, window_height / 2.f});
}
void Background::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}