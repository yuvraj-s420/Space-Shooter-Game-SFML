#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <iostream>


class Enemy {
public:

    Enemy(float w, float h, const sf::Texture& t, const sf::Texture& bt) : sprite(t), bullet_spr(bt){  // DOES NOT WORK WITHOUT constructor initializer list
        window_width = w;
        window_height = h;
    }
    void takeDamage();
    void initialize(); 
    void update(float delta);
    void setPos(sf::Vector2f pos);
    void drawEnemy(sf::RenderWindow &window);
    void createBullet();
    void updateBullets(float delta);
    void checkPlayerBulletCollision(std::vector<sf::Sprite> &player_bullets);
    void drawBullets(sf::RenderWindow &window);


    bool getAlive();
    sf::Vector2f getDesiredSize();
    std::vector<sf::Sprite>& getBullets();
    sf::Sprite& getSprite();
    sf::Vector2f getPos();
    std::string getType();

protected:
    
    float window_width;
    float window_height;
    sf::Vector2f desired_size;

    
    std::vector<sf::Sprite> bullets;
    sf::Clock bullet_clock;     // enemies own their own bullet clocks
    sf::Vector2f bullet_size = sf::Vector2f(50.f, 25.f);
    float bullet_rate = 5;             // bullets per second
    float bullet_speed = 800;          // pixels per second
    float bullet_offset = 20;
    bool shooting = true;   // currently shooting?
    int burst_count = 0;    // counter for bullet bursts

    float health;
    float speed;
    bool alive = true;
    std::string type;

    sf::Sprite sprite;
    sf::Sprite bullet_spr;
};

// -------------------- SimpleEnemy --------------------
class SimpleEnemy : public Enemy {
public:

    SimpleEnemy(float w, float h, const sf::Texture& t, const sf::Texture& bt) : Enemy(w, h, t, bt){
        type = "simple";
        health = 5.f;
        speed = 50.f;
        desired_size = sf::Vector2f(100, 100);
    } 
};

// -------------------- FastEnemy --------------------
class FastEnemy : public Enemy {
public:
    FastEnemy(float w, float h, const sf::Texture& t, const sf::Texture& bt) : Enemy(w, h, t, bt){
        type = "fast";
        health = 3.f;
        speed = 100.f;
        desired_size = sf::Vector2f(100, 100);
    }
};

// -------------------- ShootingEnemy --------------------
class ShootingEnemy : public Enemy {
public:
    ShootingEnemy(float w, float h, const sf::Texture& t, const sf::Texture& bt) : Enemy(w, h, t, bt){
        type = "shooting";
        health = 3.f;
        speed = 50.f;
        desired_size = sf::Vector2f(100, 100);
    }
};

// -------------------- BossEnemy --------------------
class BossEnemy : public Enemy {
public:
    BossEnemy(float w, float h, const sf::Texture& t, const sf::Texture& bt) : Enemy(w, h, t, bt){
        type = "boss";
        health = 50.f;
        speed = 20.f;
        desired_size = sf::Vector2f(200, 200);
    }
};
