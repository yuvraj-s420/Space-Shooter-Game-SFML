#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <random>
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "TextureManager.h"
#include "helpers.h"


// Playing field
unsigned int screen_width = 2000;     
unsigned int screen_height = 1200;

// Size of HUD showing health, score, level, pause button, etc.
int HUD_width = screen_width;
int HUD_height = 140;

int level = 1;
int score = 0;
int highscore;
int difficulty = 1; // 0, easy   1, normal   2, hard
Difficulty easy   {0.5f, 0.5f, 0.5f};
Difficulty normal {1.0f, 1.0f, 1.0f};
Difficulty hard   {2.0f, 2.0f, 2.0f};

bool spawning = true;
int base_enemies = 10;      // First level has 10 enemies
float growth_rate = 1.25;   // 50% more enemies each level
int num_enemies = base_enemies;
int wave = 1;
float spawn_timer = 0.f;
int enemies_in_wave = 1;     // starts at 1, increases each wave, triangle pattern of spawning
float wave_interval = 2.f;  // 2 seconds
float min_wave_interval = 1.f;
float wave_interval_shrink_rate = 0.1f; // wave interval reduction each level
int boss_interval = 3;      // Every 3 levels
int enemies_left = num_enemies;
int max_per_wave = 9;   

enum class GameState{
    Menu,
    Playing,
    Settings,
    Paused,
    GameOver,
};

GameState game_state = GameState::Menu;     //start at menu
GameState prev_state;

int main()
{
    srand(time(0));
    sf::Clock delta_clock;

    /*============================== INITIALIZE ==============================*/
    // Create the window
    sf::RenderWindow window(sf::VideoMode({screen_width, screen_height}), "Space Shooter Game");
    window.setFramerateLimit(60);

    // texture_manager, background, player, and enemy vector stay for the lifestyle of the program, vector gets emptied when game restarts
    TextureManager texture_manager;
    Background background(screen_width, screen_height, texture_manager.getTexture("assets/background.png"));
    Player player(screen_width, screen_height, texture_manager.getTexture("assets/spritesheet_player.png"), texture_manager.getTexture("assets/bullet.png"));
    
    // Enemies stored as unique_ptr to avoid slicing of child members
    std::vector<std::unique_ptr<Enemy>> enemies;

    // Load highscore from file into memory
    highscore = loadHighscore();

    background.initialize();
    player.initialize();
    player.setPos({player.getDesiredSize().x / 2.f, HUD_height + (screen_height - HUD_height) / 2.f});

    while (window.isOpen()){
        
        window.clear(sf::Color::Black);                             // clear previous frame

        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);    // mouse position relative to window
        float delta = delta_clock.restart().asSeconds();            // Delta time between each iteration
        bool mouse_pressed = false;

        while (const std::optional event = window.pollEvent()){
            if (event->is<sf::Event::Closed>()){    // close window
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>()) {
                mouse_pressed = true;  
            }
        }

        switch (game_state) {
            case GameState::Menu: {  // draw menu, handle menu input
                char res = drawMenu(window, mouse_pos, mouse_pressed);
                if (res == 'p'){    // start game loop
                    game_state = GameState::Playing;
                    prev_state = GameState::Menu;
                }
                else if (res == 's'){   // settings
                    game_state = GameState::Settings;
                    prev_state = GameState::Menu;
                }
                else if (res == 'q'){   // Exit window
                    if (score > highscore){     // save highscore
                        saveHighscore(score);
                    }
                    window.close();
                    prev_state = GameState::Menu;
                }
                
                break;
            }
            case GameState::Playing: {      // main game loop with HUD and pause button on top right
            
                // Updates
                spawnEnemies(enemies, texture_manager, delta, difficulty);
                player.handleInput(delta);
                player.updateBullets(delta);
                for (int i = 0; i < enemies.size(); i++){
                    player.checkEnemyBulletCollision(enemies[i]->getBullets());
                }
                updateEnemies(enemies, player, delta);
                
                // Drawing
                background.draw(window);
                player.drawPlayer(window);
                player.drawBullets(window);
                drawEnemies(enemies, window);

                // HUD draw and pause button click check
                sf::FloatRect pause_button = drawHUD(window, player, texture_manager);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || (pause_button.contains({static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y)}) && mouse_pressed)){
                    game_state = GameState::Paused;
                    prev_state = GameState::Playing;
                }

                //End game when health = 0
                if (player.getHealth() == 0.f){
                    game_state = GameState::GameOver;
                    prev_state = GameState::Playing;
                }
                
                break;
            }
            case GameState::Paused: {   // stop updates, show pause menu, handle settings, resume, quit to main menu 

                // Draw the same game frame underneath the pause menu
                background.draw(window);
                player.drawPlayer(window);
                player.drawBullets(window);
                drawEnemies(enemies, window);
                drawHUD(window, player, texture_manager);

                // Black transparent overlay
                sf::RectangleShape overlay({static_cast<float>(screen_width), static_cast<float>(screen_height)});
                overlay.setFillColor(sf::Color(0,0,0,128));
                window.draw(overlay);

                char res = drawPauseMenu(window, mouse_pos, mouse_pressed);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)){
                    game_state = GameState::Playing;
                    prev_state = GameState::Paused;
                }
                if (res == 'p'){    // start game loop
                    game_state = GameState::Playing;
                    prev_state = GameState::Paused;
                }
                else if (res == 's'){   // settings
                    game_state = GameState::Settings;
                    prev_state = GameState::Paused;
                }
                else if (res == 'q'){   // Quit to menu
                    if (score > highscore){     // save highscore
                        saveHighscore(score);
                    }
                    // Reset game variables
                    level = 1;
                    score = 0;
                    highscore = loadHighscore();
                    spawning = true;    // to refresh spawnEnemies()
                    num_enemies = base_enemies;
                    wave = 1;
                    spawn_timer = 0.f;
                    enemies_in_wave = 1;     // starts at 1, increases each wave, triangle pattern of spawning
                    enemies_left = num_enemies;

                    // clear enemies and reset player
                    player.reset();
                    enemies.clear();

                    game_state = GameState::Menu;
                    prev_state = GameState::Paused;
                }
                break;
            }

            case GameState::Settings: {     // show settings menu, handle highscore reset and difficulty selector
                char res = drawSettings(window, mouse_pos, mouse_pressed, difficulty, highscore);
                if (res == 'b'){
                    game_state = prev_state;    // returns to menu or pause state when back button is pressed
                    prev_state = GameState::Settings;
                }
                break;
            }

            case GameState::GameOver: {     // show game over screen, handle quit to main menu or quit to desktop
                // Draw the same game frame underneath the pause menu
                background.draw(window);
                player.drawPlayer(window);
                player.drawBullets(window);
                drawEnemies(enemies, window);
                drawHUD(window, player, texture_manager);

                // Black transparent overlay
                sf::RectangleShape overlay({static_cast<float>(screen_width), static_cast<float>(screen_height)});
                overlay.setFillColor(sf::Color(0,0,0,200));
                window.draw(overlay);

                char res = drawGameOver(window, mouse_pos, mouse_pressed);
                if (res == 'm'){

                    if (score > highscore){     // save highscore
                        saveHighscore(score);
                    }
                    
                    // Reset game variables
                    level = 1;
                    score = 0;
                    highscore = loadHighscore();
                    spawning = true;    // to refresh spawnEnemies()
                    num_enemies = base_enemies;
                    wave = 1;
                    spawn_timer = 0.f;
                    enemies_in_wave = 1;     // starts at 1, increases each wave, triangle pattern of spawning
                    enemies_left = num_enemies;

                    // clear enemies and reset player
                    player.reset();
                    enemies.clear();
                    game_state = GameState::Menu;
                    prev_state = GameState::GameOver;
                }
                else if (res == 'q'){
                    window.close();
                }
                break;
            }     
        }
        window.display();
    }
    return 0;
}
