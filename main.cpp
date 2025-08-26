#include <SFML/Graphics.hpp>
#include <iostream>
#include <utility>
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "TextureManager.h"

unsigned int width = 2000;
unsigned int height = 1000;

void spawnEnemies(std::vector<Enemy>& enemies, TextureManager &texture_manager, sf::Clock &spawn_clock, float &spawn_timer);
void updateEnemies(std::vector<Enemy> &enemies, Player &player, float delta);
void drawEnemies(std::vector<Enemy> &enemies, sf::RenderWindow &window);

int main()
{
    
    sf::Clock delta_clock;
    sf::Clock spawn_clock;

    /*============================== INITIALIZE ==============================*/
    // Create the window
    sf::RenderWindow window(sf::VideoMode({width, height}), "My First SFML 'Game'");
    window.setFramerateLimit(60);

    TextureManager texture_manager;
    Background background(width, height, texture_manager.getTexture("assets/background.png"));
    Player player(width, height, texture_manager.getTexture("assets/player.png"), texture_manager.getTexture("assets/bullet.png"));
    
    // Enemies
    std::vector<Enemy> enemies;

    background.initialize();
    player.initialize();

    /*============================== INITIALIZE ==============================*/

    /*============================== UPDATE ==============================*/
    while (window.isOpen()){

        float delta = delta_clock.restart().asSeconds();                 // Delta time between each iteration for movement keys
        float spawn_timer = spawn_clock.getElapsedTime().asSeconds();    // keeps track of time between enemy spawns 

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){
                window.close();
            }
            
        }


        //Enemy spawns
        spawnEnemies(enemies, texture_manager, spawn_clock, spawn_timer);

        // Player updates
        player.handleInput(delta);
        player.updateBullets(delta);
        for (int i = 0; i < enemies.size(); i++){
            player.checkEnemyBulletCollision(enemies[i].getBullets());
        }

        // Enemy updates
        updateEnemies(enemies, player, delta);

        /*============================== UPDATE ==============================*/

        /*============================== DRAW ==============================*/
        
        window.clear(sf::Color::Black);
        background.draw(window);

        player.drawPlayer(window);
        player.drawBullets(window);

        drawEnemies(enemies, window);

        window.display();
    /*============================== DRAW ==============================*/
    }

    return 0;
}



void spawnEnemies(std::vector<Enemy>& enemies, TextureManager& texture_manager, sf::Clock& spawn_clock, float& spawn_timer){
    // Spawns enemies in waves

    static int level = 1;
    static int wave = 1;

    float wave_interval = 5.f;     // spawn waves every 5 seconds
    float level_interval;

    int num_simple;
    int num_fast;
    int num_shooting;
    int num_boss;

    float spacing;


    // determined what enemies to spawn each level
    switch (level){

        case 1:
            num_simple = 3;
            num_fast = 3;
            num_shooting = 3;
            num_boss = 1;
            break;

        case 2:
            num_simple = 5;
            num_fast = 5;
            num_shooting = 5;
            num_boss = 1;
            break;

        case 3:
            num_simple = 7;
            num_fast = 7;
            num_shooting = 7;
            num_boss = 2;
            break;

        case 4:
            num_simple = 9;
            num_fast = 9;
            num_shooting = 9;
            num_boss = 2;
            break;

        case 5:
            num_simple = 9;
            num_fast = 9;
            num_shooting = 9;
            num_boss = 3;
            break;

        default:    // stays level 5 indefinitely
            num_simple = 9;
            num_fast = 9;
            num_shooting = 9;
            num_boss = 3;
            break;

    }

    if (spawn_timer > wave_interval){
        
        std::cout << "Level: " << level << "  Wave: " << wave << std::endl;

        switch (wave){
            case 1:     // spawn fast enemy
                spacing = height / (num_fast + 1);

                for (int i = 0; i < num_fast; i++){

                    FastEnemy fast_enemy(width, height, texture_manager.getTexture("assets/fast_enemy.png"), texture_manager.getTexture("assets/enemy_bullet.png"));
                    fast_enemy.initialize();
                    // Space out enemies equally
                    fast_enemy.setPos({width - fast_enemy.getDesiredSize().x / 2.f, spacing * (i + 1)});
                    enemies.push_back(fast_enemy);
                }
                wave++;
                break;

            case 2:     // spawn simple enemy
                spacing = height / (num_simple + 1);

                for (int i = 0; i < num_simple; i++){

                    SimpleEnemy simple_enemy(width, height, texture_manager.getTexture("assets/simple_enemy.png"), texture_manager.getTexture("assets/enemy_bullet.png"));
                    simple_enemy.initialize();
                    // Space out enemies equally
                    simple_enemy.setPos({width - simple_enemy.getDesiredSize().x / 2.f, spacing * (i + 1)});
                    enemies.push_back(simple_enemy);
                }
                wave++;
                break;

            case 3:     // spawn shooting enemy
                spacing = height / (num_shooting + 1);

                for (int i = 0; i < num_shooting; i++){

                    ShootingEnemy shooting_enemy(width, height, texture_manager.getTexture("assets/shooting_enemy.png"), texture_manager.getTexture("assets/enemy_bullet.png"));
                    shooting_enemy.initialize();
                    // Space out enemies equally
                    shooting_enemy.setPos({width - shooting_enemy.getDesiredSize().x / 2.f, spacing * (i + 1)});
                    enemies.push_back(shooting_enemy);
                }
                wave++;
                break;

            case 4:     // spawn boss
                spacing = height / (num_boss + 1);

                for (int i = 0; i < num_boss; i++){

                    BossEnemy boss_enemy(width, height, texture_manager.getTexture("assets/boss_enemy.png"), texture_manager.getTexture("assets/enemy_bullet.png"));
                    boss_enemy.initialize();
                    // Space out enemies equally
                    boss_enemy.setPos({width - boss_enemy.getDesiredSize().x / 2.f, spacing * (i + 1)});
                    enemies.push_back(boss_enemy);
                }
                wave = 1;   //reset wave to 1 after spawning boss
                level++;    //increment level
                break;

            default:   // should not happen
                std::cout << "Should not happen\n";
                break;
        }

        spawn_timer = spawn_clock.restart().asSeconds();    // restart spawn timer and clock to 0
    }

}   

void updateEnemies(std::vector<Enemy>& enemies, Player &player, float delta){

    for (int i = 0; i < enemies.size(); ){

        // update movement and check if they are hit by player bullets
        enemies[i].update(delta);
        enemies[i].checkPlayerBulletCollision(player.getBullets());

        // create bullets if the enemy is a shooting one
        if (enemies[i].getType() == "shooting" || enemies[i].getType() == "boss"){

            enemies[i].createBullet();    // enemies share bullet timer might be issue
            enemies[i].updateBullets(delta);
        }

        sf::Vector2f pos = enemies[i].getPos();

        // removes enemy from vector if it dies or leaves window bounds
        if (!enemies[i].getAlive() || pos.x > width || pos.x < 0 || pos.y < 0 || pos.y > height){
            enemies.erase(enemies.begin() + i);
        }
        else{
            i++; // increment here if enemy not erased to prevent index skipping
        }

        // Draw here to prevent looping again??

    }
}

void drawEnemies(std::vector<Enemy>& enemies, sf::RenderWindow &window){

    for (int i = 0; i < enemies.size(); i++){
        enemies[i].drawEnemy(window);
        enemies[i].drawBullets(window);
    }
}