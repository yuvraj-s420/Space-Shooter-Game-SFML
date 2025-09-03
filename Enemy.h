#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <iostream>

struct Difficulty {
    float enemy_health_multiplier;
    float bullet_speed_multiplier;
    float max_burst_multiplier;
};


struct EnemyBullet {
    sf::Sprite bullet_spr;
    sf::Vector2f unit_vector;

    EnemyBullet(sf::Sprite spr) : bullet_spr(spr) { }
};

class Enemy {
protected:
    
    float window_width;
    float window_height;
    sf::Vector2f desired_size;

    
    sf::Clock bullet_clock;     // enemies own their own bullet clocks
    sf::Vector2f bullet_size = sf::Vector2f(25.f, 50.f);
    float bullet_rate = 5;             // bullets per second
    float bullet_speed = 800;          // pixels per second
    float bullet_offset = 20;
    bool shooting = true;   // currently shooting?
    int burst_count = 0;    // counter for bullet bursts
    int max_burst = 5;
    float cooldown_time = 2.f;
    
    std::vector<EnemyBullet> bullets;


    float health;
    float speed;
    bool alive = true;
    std::string type;

    sf::Sprite sprite;
    sf::Sprite bullet_spr;

public:

    Enemy(float w, float h, const sf::Texture& t, const sf::Texture& bt, Difficulty difficulty) : sprite(t), bullet_spr(bt){  // DOES NOT WORK WITHOUT constructor initializer list
        window_width = w;
        window_height = h;
    }
    void takeDamage();
    void initialize(); 
    void update(float delta);
    void setPos(sf::Vector2f pos);
    void drawEnemy(sf::RenderWindow &window);
    void createBullet(sf::Vector2f player_pos);
    virtual void updateBullets(float delta);
    void checkPlayerBulletCollision(std::vector<sf::Sprite> &player_bullets);
    void drawBullets(sf::RenderWindow &window);

    bool getAlive();
    float getHealth();
    sf::Vector2f getDesiredSize();
    std::vector<EnemyBullet>& getBullets();
    sf::Sprite& getSprite();
    sf::Vector2f getPos();
    std::string getType();

};

// -------------------- SimpleEnemy --------------------
class SimpleEnemy : public Enemy {
public:

    SimpleEnemy(float w, float h, const sf::Texture& t, const sf::Texture& bt, Difficulty difficulty) : Enemy(w, h, t, bt, difficulty){
        type = "simple";
        health = 5.f;
        speed = 50.f;
        desired_size = sf::Vector2f(100, 100);

        health *= difficulty.enemy_health_multiplier;
        bullet_speed *= difficulty.bullet_speed_multiplier;
        max_burst *= difficulty.max_burst_multiplier;
    } 
};

// -------------------- FastEnemy --------------------
class FastEnemy : public Enemy {
public:
    FastEnemy(float w, float h, const sf::Texture& t, const sf::Texture& bt, Difficulty difficulty) : Enemy(w, h, t, bt, difficulty){
        type = "fast";
        health = 3.f;
        speed = 100.f;
        desired_size = sf::Vector2f(100, 100);
        health *= difficulty.enemy_health_multiplier;
        bullet_speed *= difficulty.bullet_speed_multiplier;
        max_burst *= difficulty.max_burst_multiplier;
    }
};

// -------------------- ShootingEnemy --------------------
class ShootingEnemy : public Enemy {
public:
    ShootingEnemy(float w, float h, const sf::Texture& t, const sf::Texture& bt, Difficulty difficulty) : Enemy(w, h, t, bt, difficulty){
        type = "shooting";
        health = 3.f;
        speed = 50.f;
        desired_size = sf::Vector2f(100, 100);
        health *= difficulty.enemy_health_multiplier;
        bullet_speed *= difficulty.bullet_speed_multiplier;
        max_burst *= difficulty.max_burst_multiplier;
    }
};

// -------------------- BossEnemy --------------------
class BossEnemy : public Enemy {
public:
    BossEnemy(float w, float h, const sf::Texture& t, const sf::Texture& bt, Difficulty difficulty) : Enemy(w, h, t, bt, difficulty){
        type = "boss";
        health = 25.f;
        speed = 20.f;
        max_burst = 15;
        desired_size = sf::Vector2f(200, 200);
        health *= difficulty.enemy_health_multiplier;
        bullet_speed *= difficulty.bullet_speed_multiplier;
        max_burst *= difficulty.max_burst_multiplier;
    }
    void updateBullets(float delta) override;

};
