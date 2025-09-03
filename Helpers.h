#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>
#include <fstream>
#include <random>
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "TextureManager.h"

extern std::random_device rd;
extern std::mt19937 gen;
extern std::uniform_int_distribution<> distrib;

int loadHighscore();
void saveHighscore(int num);
char drawPauseMenu(sf::RenderWindow &window, sf::Vector2i mouse_pos, bool mouse_pressed);
char drawMenu(sf::RenderWindow &window, sf::Vector2i mouse_pos, bool mouse_pressed);
sf::FloatRect drawHUD(sf::RenderWindow &window, Player &player, TextureManager texture_manager);
void spawnEnemies(std::vector<std::unique_ptr<Enemy>> &enemies, TextureManager &texture_manager, float delta, int difficulty);
void updateEnemies(std::vector<std::unique_ptr<Enemy>> &enemies, Player &player, float delta);
void drawEnemies(std::vector<std::unique_ptr<Enemy>> &enemies, sf::RenderWindow &window);
char drawGameOver(sf::RenderWindow &window, sf::Vector2i mouse_pos, bool mouse_pressed);
char drawSettings(sf::RenderWindow &window, sf::Vector2i mouse_pos, bool mouse_pressed, int &difficulty, int &highscore);