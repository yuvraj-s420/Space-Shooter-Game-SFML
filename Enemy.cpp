#include "Enemy.h"
#include <algorithm>
#include <cmath>

// ENEMY
void Enemy::takeDamage(){

    if (health > 0){
        health -= 1;
    }
    
    if (health <= 0){
        health = 0;
        alive = false;
    }
}

bool Enemy::getAlive(){
    return alive;
}

void Enemy::initialize(){
    // Scales sprite according to the desired size, and sets its origin to the center

    sf::Vector2u sizeU = sprite.getTexture().getSize();
    sf::Vector2f center(sizeU.x / 2.f, sizeU.y / 2.f);  // vector2f center cooridnate because setOrigin requires it
    sf::Vector2f scale_factors(desired_size.x / sizeU.x, desired_size.y / sizeU.y);

    sprite.setScale(scale_factors);
    sprite.setOrigin(center);
    sprite.setRotation(sf::degrees(90));
}

void Enemy::setPos(sf::Vector2f pos){
    sprite.setPosition(pos);
}

void Enemy::update(float delta){
    // Updates the enemy's position by moving it to the left
    sprite.move({-speed * delta, 0.f});
}

void Enemy::drawBullets(sf::RenderWindow &window){

    for (int i = 0; i < bullets.size(); i++){
        window.draw(bullets[i].bullet_spr);
    }
}

void Enemy::createBullet(sf::Vector2f player_pos){
    // Creates an EnemyBullet object and adds it to the bullets vector
    // Shoots a burst of 5 bullets with a cooldown of 2 seconds

    float bullet_timer = bullet_clock.getElapsedTime().asSeconds();

    if (shooting){

        if (bullet_timer >= (1 / bullet_rate)){

            sf::Sprite bullet = bullet_spr;
            EnemyBullet enemy_bullet = {bullet_spr};

            sf::Vector2u sizeU = bullet.getTexture().getSize();
            sf::Vector2f center(sizeU.x / 2.f, sizeU.y / 2.f);  // vector2f center cooridnate because setOrigin requires it
            sf::Vector2f scale_factors(bullet_size.x / sizeU.x, bullet_size.y / sizeU.y);

            bullet.setScale(scale_factors);
            bullet.setOrigin(center);
            bullet.setPosition(sprite.getPosition() - sf::Vector2f((desired_size.x + bullet_size.x) / 2.f - bullet_offset, 0.f));     //bullet drawn left of sprite

            // Vector from bullet to player
            sf:: Vector2f bullet_unit_vector = player_pos - bullet.getPosition();
            float mag = std::sqrt(bullet_unit_vector.x * bullet_unit_vector.x + bullet_unit_vector.y * bullet_unit_vector.y);
            bullet_unit_vector /= mag;

            // convert to degrees, add 90 because rotations start from positive y
            float angle = 90 + std::atan2(bullet_unit_vector.y, bullet_unit_vector.x) * 180 / 3.1415;  
            
            //std::cout << angle << std::endl;

            // boss bullets target player, others just shoot left
            if (type == "boss"){
                bullet.setRotation(sf::degrees(angle));
            }
            else{
                bullet.setRotation(sf::degrees(-90));
            }
            
            enemy_bullet.bullet_spr = bullet;
            enemy_bullet.unit_vector = bullet_unit_vector;

            bullets.push_back(enemy_bullet);
            burst_count++;

            bullet_timer = bullet_clock.restart().asSeconds();
        }

    }

    if (burst_count >= max_burst){
        shooting = false;
        burst_count = 0;
        bullet_timer = bullet_clock.restart().asSeconds();
    }

    if (bullet_timer >= cooldown_time){
        shooting = true;
    }
    
}

void Enemy::updateBullets(float delta){
    // Updates each bullet to move to the right (for non boss enemies), and erases if it leaves window bounds

    for (int i = 0; i < bullets.size(); ){

        bullets[i].bullet_spr.move({-1.f * delta * bullet_speed, 0.f});

        // Erase bullet if it leaves window bounds
        sf::Vector2f pos = bullets[i].bullet_spr.getPosition();
        if (pos.x > window_width || pos.x < 0 || pos.y < 0 || pos.y > window_height){
            bullets.erase(bullets.begin() + i);
        }
        else {
            i++;    // Increment i here to prevent skipping index from bullets.erase
        }

    }

}

void Enemy::checkPlayerBulletCollision(std::vector<sf::Sprite>& player_bullets) {
    // checks for collision with player bullets and takes damage
    
    sf::FloatRect enemy_bb = sprite.getGlobalBounds();

    if (alive){
        for (int i = 0; i < player_bullets.size(); ) {
            sf::FloatRect bullet_bb = player_bullets[i].getGlobalBounds();

            if (const std::optional intersection = enemy_bb.findIntersection(bullet_bb)) {
                takeDamage();
                player_bullets.erase(player_bullets.begin() + i); // remove bullet so it can’t damage again
                break; // enemy only takes 1 hit per frame
            }
            else {
                i++;
            }
        }
    }
    
}

void Enemy::drawEnemy(sf::RenderWindow &window){

    window.draw(sprite);
    
}

sf::Vector2f Enemy::getDesiredSize(){
    return desired_size;
}

std::vector<EnemyBullet>& Enemy::getBullets(){
    return bullets;
}

sf::Sprite& Enemy::getSprite(){
    return sprite;
}
sf::Vector2f Enemy::getPos(){
    return sprite.getPosition();
}

std::string Enemy::getType(){
    return type;
}


void BossEnemy::updateBullets(float delta){
    // Updates each bullet to move to the right, and erases if it leaves window bounds

    for (int i = 0; i < bullets.size(); ){
        
        bullets[i].bullet_spr.move({1.f * delta * bullet_speed * bullets[i].unit_vector.x, 1.f * delta * bullet_speed * bullets[i].unit_vector.y});

        // Erase bullet if it leaves window bounds
        sf::Vector2f pos = bullets[i].bullet_spr.getPosition();
        if (pos.x > window_width || pos.x < 0 || pos.y < 0 || pos.y > window_height){
            bullets.erase(bullets.begin() + i);
        }
        else {
            i++;    // Increment i here to prevent skipping index from bullets.erase
        }

    }
}

float Enemy::getHealth(){
    return health;
}

// SHOOTING ENEMY
