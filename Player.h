#pragma once
#include <SFML/Graphics.hpp>

class Player {

public:

    Player(float w, float h, const sf::Texture& t, const sf::Texture& bt) : sprite(t), bullet_spr(bt){  // DOES NOT WORK WITHOUT constructor initializer list
        window_width = w;
        window_height = h;
    }
    void initialize();
    void handleInput(float delta);
    void takeDamage();
    void createBullet();
    void updateBullets(float delta);
    void drawPlayer(sf::RenderWindow &window);
    void drawBullets(sf::RenderWindow &window);
    void checkEnemyBulletCollision(std::vector<sf::Sprite> &enemy_bullets);

    float getBulletSpeed();
    float getBulletSize();
    std::vector<sf::Sprite>& getBullets();

private:

    float window_width;
    float window_height;
    sf::Vector2f desired_size = sf::Vector2f(100.f, 100.f);

    std::vector<sf::Sprite> bullets;
    float bullet_size = 50;
    float bullet_rate = 5;             // bullets per second
    float bullet_speed = 800;          // pixels per second
    float bullet_offset = 20;
    sf::Clock bullet_clock;

    float move_speed = 500.f;
    float health = 10.f;
    bool alive = true;

    sf::Sprite sprite;
    sf::Sprite bullet_spr;



};
