#include "Player.h"
#include <iostream>
#include <algorithm>

void Player::initialize(){
    // sets player sprite according to desired size, and sets its origin to the center

    sf::Vector2u sizeU = sprite.getTexture().getSize();
    sf::Vector2f center(sizeU.x / 2.f, sizeU.y / 2.f);  // vector2f center cooridnate because setOrigin requires it
    sf::Vector2f scale_factors(desired_size.x / sizeU.x, desired_size.y / sizeU.y);

    sprite.setScale(scale_factors);
    sprite.setOrigin(center);

    sprite.setPosition({50 + desired_size.x / 2.f, window_height / 2.f});
    sprite.setRotation(sf::degrees(90));


}

void Player::handleInput(float delta){
    // handles movement inputs WASD, and shooting

    float bullet_timer = bullet_clock.getElapsedTime().asSeconds();

    // Move player based on key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)){
        sprite.move({0.f, -1.f * move_speed * delta});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)){
        sprite.move({0.f, 1.f * move_speed * delta});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)){
        sprite.move({1.f * move_speed * delta, 0.f});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)){
        sprite.move({-1.f * move_speed * delta, 0.f});
    }
    // Clamp position within window
    float x = std::max(desired_size.x / 2.f, std::min(sprite.getPosition().x, window_width - desired_size.x / 2.f));
    float y = std::max(desired_size.y / 2.f, std::min(sprite.getPosition().y, window_height - desired_size.y / 2.f));
    sprite.setPosition({x, y});

    // Shoot bullet with E, according to rate 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E) && bullet_timer >= (1 / bullet_rate)){

        createBullet();

        bullet_timer = bullet_clock.restart().asSeconds();
    }

    updateBullets(delta);
}

void Player::takeDamage(){

    if (health > 0){
        health -= 1;
    }
    
    if (health == 0){
        alive = false;
    }
}

void Player::drawPlayer(sf::RenderWindow &window)
{
    window.draw(sprite);
}

void Player::drawBullets(sf::RenderWindow &window){

    for (int i = 0; i < bullets.size(); i++){
        window.draw(bullets[i]);
    }
}

void Player::createBullet(){
    // Creates a bullet sprite and adds it to the bullets vector
    
    sf::Sprite bullet = bullet_spr;
    sf::Vector2u sizeU = bullet.getTexture().getSize();
    sf::Vector2f center(sizeU.x / 2.f, sizeU.y / 2.f);  // vector2f center cooridnate because setOrigin requires it
    sf::Vector2f scale_factors(bullet_size / sizeU.x, bullet_size / sizeU.y);

    bullet.setScale(scale_factors);
    bullet.setOrigin(center);
    bullet.setPosition(sprite.getPosition() + sf::Vector2f((desired_size.x + bullet_size) / 2.f - bullet_offset, 0.f));     //bullet drawn right of sprite
    bullet.setRotation(sf::degrees(90));

    bullets.push_back(bullet);
    //bullets[bullets.size() - 1].setPosition(sprite.getPosition() + sf::Vector2f((desired_size.x + bullet_size) / 2.f - bullet_offset, 0.f));

}

void Player::updateBullets(float delta){
    // Updates each bullet to move to the right

    for (int i = 0; i < bullets.size(); ){

        bullets[i].move({1.f * delta * bullet_speed, 0.f});

        // Erase bullet if it leaves window bounds
        sf::Vector2f pos = bullets[i].getPosition();
        if (pos.x > window_width || pos.x < 0 || pos.y < 0 || pos.y > window_height){
            bullets.erase(bullets.begin() + i);
        }
        else {
            i++;    // Increment i here to prevent skipping index from bullets.erase
        }

    }

}

void Player::checkEnemyBulletCollision(std::vector<sf::Sprite>& enemy_bullets) {
    // checks for collision with enemy bullets and takes damage
    
    sf::FloatRect enemy_bb = sprite.getGlobalBounds();

    if (alive){
        for (int i = 0; i < enemy_bullets.size(); ) {
            sf::FloatRect bullet_bb = enemy_bullets[i].getGlobalBounds();

            if (const std::optional intersection = enemy_bb.findIntersection(bullet_bb)) {
                takeDamage();
                std::cout << "Player hit! Health: " << health << std::endl;

                enemy_bullets.erase(enemy_bullets.begin() + i); // remove bullet so it can’t damage again
                break; // player only takes 1 hit per frame
            }
            else {
                i++;
            }
        }
    }
    
}

std::vector<sf::Sprite>& Player::getBullets(){
    return bullets;
}

float Player::getBulletSpeed(){
    return bullet_speed;
}

float Player::getBulletSize(){
    return bullet_size;
}