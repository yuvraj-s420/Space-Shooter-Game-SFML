#pragma once
#include <SFML/Graphics.hpp>

class Background{

public:

    Background(float w, float h,const sf::Texture& t) : texture(t), sprite(texture) {     // DOES NOT WORK WITHOUT constructor initializer list
        window_width = w;
        window_height = h;
    }
    void initialize();
    void draw(sf::RenderWindow &window);

private:
    
    const sf::Texture texture;
    sf::Sprite sprite;
    float window_width;
    float window_height;

};