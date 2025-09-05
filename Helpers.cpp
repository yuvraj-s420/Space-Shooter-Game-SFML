#include "helpers.h"
#include <random>

std::random_device rd;      // seed generator
std::mt19937 gen(rd());     // random engine
std::uniform_int_distribution<> distrib(1, 3);  // numbers 1–3 for types of enemy

// initialized in main
extern unsigned int screen_width;
extern unsigned int screen_height;
extern int HUD_width;
extern int HUD_height;
extern int level;
extern int score;
extern int highscore;
extern bool spawning;
extern int base_enemies;
extern float growth_rate;
extern int num_enemies;
extern int wave;
extern float spawn_timer;
extern int enemies_in_wave;
extern float wave_interval;
extern float min_wave_interval;
extern float wave_interval_shrink_rate;
extern int boss_interval;
extern int enemies_left;
extern int max_per_wave;

extern Difficulty easy;
extern Difficulty normal;
extern Difficulty hard;

int loadHighscore(){

    if (!std::filesystem::exists("data")) {
        std::filesystem::create_directory("data");
    }
    std::ifstream in("data/highscore.txt");

    int highscore;
    if (in.is_open()){
        in >> highscore;
        in.close();
    }
    else{
        // File does not exist, so create it with default 0
        std::ofstream out("data/highscore.txt");
        if (out.is_open()) {
            out << 0;
            out.close();
        }
    }
    return highscore;
}

void saveHighscore(int num){
    std::ofstream out("data/highscore.txt");

    if(out.is_open()){
        out << num;
        out.close();
    }
    else{
    std::cout << "Error opening highscore file" << std::endl;
    }
}

char drawPauseMenu(sf::RenderWindow &window, sf::Vector2i mouse_pos, bool mouse_pressed) {
    
    static bool initialized = false;

    // Persistent elements
    static sf::Font font("assets/arial.ttf");
    static sf::Text title(font);
    static std::vector<sf::RectangleShape> buttons;
    std::vector<std::string> options = {"CONTINUE GAME", "SETTINGS", "QUIT"};
    static std::vector<sf::Text> button_texts;

    // Button settings
    static const float button_width = 400.f;
    static const float button_height = 70.f;
    static const float spacing = 30.f;
    static const float start_y = 450.f;

    if (!initialized) {
        // === Title ===
        title.setString("Paused");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color(0, 200, 255));
        title.setPosition({(window.getSize().x - title.getLocalBounds().size.x) / 2.f, 300.f});

        // === Buttons ===

        for (int i = 0; i < options.size(); i++) {
            sf::RectangleShape button({button_width, button_height});
            float button_x = (window.getSize().x - button_width) / 2.f;
            float button_y = start_y + i * (button_height + spacing);
            button.setPosition({button_x, button_y});
            button.setFillColor(sf::Color(10, 10, 40, 200));
            button.setOutlineThickness(4);
            button.setOutlineColor(sf::Color(0, 200, 255, 150));
            buttons.push_back(button);

            sf::Text option_text(font);
            option_text.setString(options[i]);
            option_text.setCharacterSize(28);
            option_text.setFillColor(sf::Color::White);
            option_text.setPosition({button_x + (button_width - option_text.getLocalBounds().size.x) / 2.f, button_y + (button_height - option_text.getLocalBounds().size.y) / 2.f - 8.f});
            button_texts.push_back(option_text);
        }

        initialized = true;
    }

    // === Draw ===
    window.draw(title);
    for (int i = 0; i < buttons.size(); i++) {
        window.draw(buttons[i]);
        window.draw(button_texts[i]);
    }

    // Check mouse hover + click
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].getGlobalBounds().contains({static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y)})) {
            buttons[i].setOutlineColor(sf::Color(0, 255, 180)); // hover highlight
            if (mouse_pressed) {
                if (i == 0){
                    return 'p'; // Play
                } 
                else if (i == 1){
                    return 's'; // Settings
                }
                else if (i == 2){
                    return 'q'; // Quit
                }
            }
        } 
        else{   // Reset outline color for unselected buttons
            buttons[i].setOutlineColor(sf::Color(0, 200, 255, 150));
        }
    }
    

    return -1;  //error should not happen
}

char drawMenu(sf::RenderWindow &window, sf::Vector2i mouse_pos, bool mouse_pressed) {
    
    static bool initialized = false;

    // Persistent elements
    static sf::Font font("assets/arial.ttf");
    static sf::Text title(font);
    static std::vector<sf::RectangleShape> buttons;
    std::vector<std::string> options = {"START GAME", "SETTINGS", "QUIT"};
    static std::vector<sf::Text> button_texts;
    static sf::Text credit(font);

    // Button settings
    static const float button_width = 400.f;
    static const float button_height = 70.f;
    static const float spacing = 30.f;
    static const float start_y = 450.f;

    if (!initialized) {
        // === Title ===
        title.setString("SPACE SHOOTER");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color(0, 200, 255));
        title.setPosition({(window.getSize().x - title.getLocalBounds().size.x) / 2.f, 300.f});

        // === Buttons ===

        for (int i = 0; i < options.size(); i++) {
            sf::RectangleShape button({button_width, button_height});
            float button_x = (window.getSize().x - button_width) / 2.f;
            float button_y = start_y + i * (button_height + spacing);
            button.setPosition({button_x, button_y});
            button.setFillColor(sf::Color(10, 10, 40, 200));
            button.setOutlineThickness(4);
            button.setOutlineColor(sf::Color(0, 200, 255, 150));
            buttons.push_back(button);

            sf::Text option_text(font);
            option_text.setString(options[i]);
            option_text.setCharacterSize(28);
            option_text.setFillColor(sf::Color::White);
            option_text.setPosition({button_x + (button_width - option_text.getLocalBounds().size.x) / 2.f, button_y + (button_height - option_text.getLocalBounds().size.y) / 2.f - 8.f});
            button_texts.push_back(option_text);
        }

        // === Credit ===
        credit.setString("Created by: Yuvraj Sandhu");
        credit.setCharacterSize(20);
        credit.setFillColor(sf::Color(150, 150, 255, 180));
        credit.setPosition({20.f, window.getSize().y - 40.f});

        initialized = true;
    }

    // === Draw ===
    window.clear(sf::Color(5, 5, 30));
    window.draw(title);
    for (int i = 0; i < buttons.size(); i++) {
        window.draw(buttons[i]);
        window.draw(button_texts[i]);
    }
    window.draw(credit);

    // Check mouse hover + click
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].getGlobalBounds().contains({static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y)})) {
            buttons[i].setOutlineColor(sf::Color(0, 255, 180)); // hover highlight
            if (mouse_pressed) {
                if (i == 0){
                    return 'p'; // Play
                } 
                else if (i == 1){
                    return 's'; // Settings
                }
                else if (i == 2){
                    return 'q'; // Quit
                }
            }
        } 
        else{   // Reset outline color for unselected buttons
            buttons[i].setOutlineColor(sf::Color(0, 200, 255, 150));
        }
    }
    

    return -1;  //error should not happen
}

sf::FloatRect drawHUD(sf::RenderWindow &window, Player &player, TextureManager texture_manager) {
    // HUD showing player health, score, high score, level, and pause button
    // initializes once to prevent lag, and updates/draws every other time
    // returns bounding box of pause button


    static bool initialized = false;

    static sf::Font font("assets/arial.ttf");
    static sf::RectangleShape player_info_box, health_bar, health_border, level_box;
    static sf::Text health_label(font), score_text(font), high_score_text(font), level_text(font);
    static sf::Texture pause_tex(texture_manager.getTexture("assets/pause.png"));
    static sf::Sprite pause_button(pause_tex);

    // Constants
    static const int hud_height = 140;
    static const int health_bar_width = 200;
    static const int health_bar_height = 25;
    static const int player_info_box_width = 450;
    static const float hud_padding = 50.f;
    static const float thickness = 8.f;

    if (!initialized){      // initializes once to prevent lag and drop in framerate

        // === LEFT BOX ===
        player_info_box.setSize({(float)player_info_box_width, (float)hud_height});
        player_info_box.setFillColor(sf::Color(10, 10, 40, 200));
        player_info_box.setOutlineThickness(thickness);
        player_info_box.setOutlineColor(sf::Color(0, 200, 255, 180));
        player_info_box.setPosition({hud_padding, hud_padding});

        float player_info_box_center_y = player_info_box.getPosition().y + hud_height / 2.f;

        // HEALTH BORDER
        health_border.setSize({(float)health_bar_width, (float)health_bar_height});
        health_border.setFillColor(sf::Color::Transparent);
        health_border.setOutlineThickness(2);
        health_border.setOutlineColor(sf::Color::White);
        health_border.setPosition({player_info_box.getPosition().x + 20.f, player_info_box_center_y - health_bar_height / 2.f});

        // HEALTH BAR (will update each frame)
        health_bar.setPosition(health_border.getPosition());

        // HEALTH LABEL
        health_label.setFont(font);
        health_label.setString("HEALTH");
        health_label.setCharacterSize(18);
        health_label.setFillColor(sf::Color::White);
        health_label.setPosition({player_info_box.getPosition().x + 20.f, player_info_box_center_y - 50.f});

        // SCORE TEXTS (will update each frame)
        float score_x = player_info_box.getPosition().x + 20.f + health_bar_width + 30.f;
        score_text.setFont(font);
        score_text.setCharacterSize(20);
        score_text.setFillColor(sf::Color::White);
        score_text.setPosition({score_x, player_info_box_center_y - 20.f});

        high_score_text.setFont(font);
        high_score_text.setCharacterSize(20);
        high_score_text.setFillColor(sf::Color(255, 215, 0));
        high_score_text.setPosition({score_x, player_info_box_center_y + 10.f});

        // === MIDDLE BOX ===
        int level_box_width = 300;
        level_box.setSize({(float)level_box_width, (float)hud_height});
        level_box.setFillColor(sf::Color(10, 10, 40, 200));
        level_box.setOutlineThickness(thickness);
        level_box.setOutlineColor(sf::Color(0, 200, 255, 180));
        level_box.setPosition({(window.getSize().x - level_box_width) / 2.f, hud_padding});

        // LEVEL TEXT (updates each frame)
        level_text.setFont(font);
        level_text.setCharacterSize(22);
        level_text.setFillColor(sf::Color(0, 200, 255));

        // === RIGHT BOX (Pause Button) ===
        float pause_size = 100.f;
        pause_button.setScale({pause_size / pause_button.getTexture().getSize().x, pause_size / pause_button.getTexture().getSize().y});
        pause_button.setPosition({(float)window.getSize().x - pause_size - hud_padding, hud_padding});

        initialized = true;
    }

    // === Update dynamic parts ===
    float health_percent = player.getHealth() / player.getMaxHealth();
    sf::Color health_color = (health_percent > 0.5f) ? sf::Color::Green :
                             (health_percent > 0.25f) ? sf::Color::Yellow :
                             sf::Color::Red;
    health_bar.setSize({health_bar_width * health_percent, static_cast<float>(health_bar_height)});
    health_bar.setFillColor(health_color);

    score_text.setString("SCORE: " + std::to_string(score));
    high_score_text.setString("HIGH SCORE: " + std::to_string(highscore));

    level_text.setString("LEVEL " + std::to_string(level));
    float level_text_width = level_text.getLocalBounds().size.x;
    level_text.setPosition({level_box.getPosition().x + (level_box.getSize().x - level_text_width) / 2.f, level_box.getPosition().y + hud_height / 2.f - 11.f});

    // === Draw everything ===
    window.draw(player_info_box);
    window.draw(health_bar);
    window.draw(health_border);
    window.draw(health_label);
    window.draw(score_text);
    window.draw(high_score_text);
    window.draw(level_box);
    window.draw(level_text);
    window.draw(pause_button);

    return pause_button.getGlobalBounds();
}

void spawnEnemies(std::vector<std::unique_ptr<Enemy>> &enemies, TextureManager &texture_manager, float delta, int difficulty){
    // Spawns enemies infinitely, getting progresively harder each level
    // Spawn number progresses exponentially, at 25% more each level
    // Periodicaly spawns bosses at boss intervals

    float spacing;
    Difficulty dif;
    if (difficulty == 0){
        dif = easy;
    }
    else if(difficulty == 1){
        dif = normal;
    }
    else{
        dif = hard;
    }

    if(spawning){

        if (level % boss_interval != 0){  // spawn bosses every 5 level interval

            if (spawn_timer >= wave_interval){      // only spawn every wave_interval

                float spacing = (screen_height - HUD_height) / (enemies_in_wave + 1);     // careful for div by 0 errors

                //std::cout << "Level: " << level << std::endl;
                //std::cout << "Enemies left to spawn: " << " " << enemies_left << std::endl;

                // Spawn however many enemies needed in the wave
                for (int i = 0; i < enemies_in_wave; i++){

                    std::unique_ptr<Enemy> enemy;
                    
                    // create random enemy
                    int type = distrib(gen);

                    switch (type){
                        case 1:
                            enemy = std::make_unique<SimpleEnemy>(screen_width, screen_height, texture_manager.getTexture("assets/simple_enemy.png"), texture_manager.getTexture("assets/enemy_bullet.png"), dif);
                            break;
                        case 2:
                            enemy = std::make_unique<FastEnemy>(screen_width, screen_height, texture_manager.getTexture("assets/fast_enemy.png"), texture_manager.getTexture("assets/enemy_bullet.png"),dif);
                            break;
                        case 3:
                            enemy = std::make_unique<ShootingEnemy>(screen_width, screen_height, texture_manager.getTexture("assets/shooting_enemy.png"), texture_manager.getTexture("assets/enemy_bullet.png"),dif);
                            break;
                    }

                    enemy->initialize();
                    enemy->setPos({screen_width - enemy->getDesiredSize().x / 2.f, HUD_height + spacing * (i + 1)});
                    enemies.push_back(std::move(enemy));    // move enemy to the enemies vector
                    enemies_left--;
                }

                if (enemies_left == 0){
                    spawning = false;       // Stops spawning after all waves have been spawned
                    
                }
                else{
                    enemies_in_wave++;

                    // Don't spawn more enememies than what is left, and dont spawn more than the max
                    if (enemies_in_wave > enemies_left){
                        enemies_in_wave = enemies_left;
                    }
                    if (enemies_in_wave > max_per_wave){
                        enemies_in_wave = max_per_wave;
                    }
                    wave++;
                }

                spawn_timer = 0.f;  //reset spawn timer
            }
        }
        else{

            int num_boss = level / boss_interval;

            float spacing = (screen_height - HUD_height) / (num_boss + 1);

            //std::cout << "Boss Level" << std::endl;

            for (int i = 0; i < num_boss; i++){
                std::unique_ptr<Enemy> boss = std::make_unique<BossEnemy>(screen_width, screen_height, texture_manager.getTexture("assets/boss_enemy.png"), texture_manager.getTexture("assets/enemy_bullet.png"),dif);
                boss->initialize();
                boss->setPos({screen_width - boss->getDesiredSize().x / 2.f, HUD_height + spacing * (i + 1)});
                enemies.push_back(std::move(boss));
            }

            spawning = false;
        }
    }
    else{       
        if (enemies.empty()){     // increment level and continue spawning if all enemies have been killed or left the window
            level++;
            spawning = true;
            enemies_in_wave = 1;    // reset number of enemies in wave to 1
            wave = 1;               // reset wave to 1
            wave_interval = std::max(min_wave_interval, wave_interval * (1 - wave_interval_shrink_rate));

            num_enemies = static_cast<int>(num_enemies * growth_rate); // next level will have more enemies
            enemies_left = num_enemies;
        }

    }

    spawn_timer += delta;

}   

void updateEnemies(std::vector<std::unique_ptr<Enemy>> &enemies, Player &player, float delta){
    // Updates each enemy along with its bullets, and erases it from the vector if it dies or leaves window bounds
    // levels up player bullets after a boss is killed in a boss round (once per round)
    static bool leveled = false;
    for (int i = 0; i < enemies.size(); ){

        // update movement and check if they are hit by player bullets
        enemies[i]->update(delta);
        enemies[i]->checkPlayerBulletCollision(player.getBullets());

        // create bullets if the enemy is a shooting one
        if (enemies[i]->getType() == "shooting" || enemies[i]->getType() == "boss"){

            enemies[i]->createBullet(player.getPos());    // enemies share bullet timer might be issue
            enemies[i]->updateBullets(delta);
        }

        sf::Vector2f pos = enemies[i]->getPos();

        // removes enemy from vector if it dies or leaves window bounds
        if (pos.x > screen_width || pos.x < 0 || pos.y < 0 || pos.y > screen_height){
            enemies.erase(enemies.begin() + i);
        }
        else if (!enemies[i]->getAlive()){

            if (enemies[i]->getType() == "boss"){   // bosses give 10 points, regular enemies give 1
                score += 10;
                
                if (!leveled){      // Drop powerup once per boss level
                    player.levelUp();
                    leveled = true;
                }
                
            }
            else{
                score++;
            }
            enemies.erase(enemies.begin() + i);
        }
        else{
            i++; // increment here if enemy not erased to prevent index skipping
        }
        // Draw here to prevent looping again??

    }

    if (enemies.empty()){   // reset leveled flag once all enemies are dead
        leveled = false;
    }
    
}

void drawEnemies(std::vector<std::unique_ptr<Enemy>> &enemies, sf::RenderWindow &window){


    for (int i = 0; i < enemies.size(); i++){
        enemies[i]->drawEnemy(window);
        enemies[i]->drawBullets(window);
    }
}

char drawGameOver(sf::RenderWindow &window, sf::Vector2i mouse_pos, bool mouse_pressed){
    
    static bool initialized = false;
    static sf::Font font("assets/arial.ttf");
    static sf::Text title_text(font);
    static sf::Text score_text(font);
    static sf::Text highscore_text(font);

    static sf::RectangleShape button_menu;
    static sf::RectangleShape button_quit;
    static sf::Text text_menu(font);
    static sf::Text text_quit(font);

    static const float button_width = 400.f;
    static const float button_height = 70.f;
    static const float spacing = 30.f;

    if (!initialized) {
        if (!font.openFromFile("assets/arial.ttf")) {
            std::cerr << "Failed to load font\n";
        }

        // Title
        title_text.setString("GAME OVER");
        title_text.setCharacterSize(80);
        title_text.setFillColor(sf::Color::Red);
        title_text.setStyle(sf::Text::Bold);
        title_text.setPosition({
            screen_width / 2.f - title_text.getGlobalBounds().size.x / 2.f,
            screen_height / 5.f}
        );

        // Score
        score_text.setCharacterSize(40);
        score_text.setFillColor(sf::Color::White);

        // High Score
        highscore_text.setCharacterSize(40);
        highscore_text.setFillColor(sf::Color::Yellow);

        // Buttons
        button_menu.setSize({button_width, button_height});
        button_menu.setFillColor(sf::Color(10, 10, 40, 200));
        button_menu.setOutlineThickness(3);
        button_menu.setOutlineColor(sf::Color(0, 200, 255, 150));
        button_menu.setPosition(
            {screen_width / 2.f - button_width - spacing / 2.f,
            screen_height / 2.f + 100.f}
        );

        text_menu.setString("Quit to Menu");
        text_menu.setCharacterSize(30);
        text_menu.setFillColor(sf::Color::White);
        text_menu.setPosition(
            {button_menu.getPosition().x + button_menu.getSize().x / 2.f - text_menu.getGlobalBounds().size.x / 2.f,
            button_menu.getPosition().y + 10.f}
        );

        button_quit.setSize({button_width, button_height});
        button_quit.setFillColor(sf::Color(10, 10, 40, 200));
        button_quit.setOutlineThickness(3);
        button_quit.setOutlineColor(sf::Color(0, 200, 255, 150));
        button_quit.setPosition({
            screen_width / 2.f + spacing / 2.f,
            screen_height / 2.f + 100.f}
        );

        text_quit.setString("Quit to Desktop");
        text_quit.setCharacterSize(30);
        text_quit.setFillColor(sf::Color::White);
        text_quit.setPosition(
            {button_quit.getPosition().x + button_quit.getSize().x / 2.f - text_quit.getGlobalBounds().size.x / 2.f,
            button_quit.getPosition().y + 10.f}
        );

        initialized = true;
    }

    // Dynamically update score and highscores
    score_text.setString("Score: " + std::to_string(score));
    score_text.setPosition({
        screen_width / 2.f - score_text.getGlobalBounds().size.x / 2.f,
        screen_height / 2.f - 100.f}
    );

    std::string str;
    if (score > highscore){
        str = "New High Score! " + std::to_string(score);
    } else {
        str = "High Score: " + std::to_string(highscore);
    }
    highscore_text.setString(str);
    highscore_text.setPosition({
        screen_width / 2.f - highscore_text.getGlobalBounds().size.x / 2.f,
        screen_height / 2.f - 40.f}
    );

    // === Draw UI Elements ===
    window.draw(title_text);
    window.draw(score_text);
    window.draw(highscore_text);

    window.draw(button_menu);
    window.draw(text_menu);
    window.draw(button_quit);
    window.draw(text_quit);


    if (button_menu.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos))){
        button_menu.setOutlineColor(sf::Color(0, 255, 180)); // hover highlight
        if (mouse_pressed){
            return 'm';
        }
    }
    else {
        button_menu.setOutlineColor(sf::Color(0, 200, 255, 150));
    }

    if (button_quit.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos))){
        button_quit.setOutlineColor(sf::Color(0, 255, 180)); // hover highlight
        if (mouse_pressed){
            return 'q';
        }
    }
    else{
        button_quit.setOutlineColor(sf::Color(0, 200, 255, 150));
    }

    return -1;      //error sould not occur
}

void resetHighscore(){     
    //reset highscore to 0
    saveHighscore(0);
}

char drawSettings(sf::RenderWindow &window, sf::Vector2i mouse_pos, bool mouse_pressed, int &difficulty, int &highscore) {
    // difficulty: 0 = Easy, 1 = Normal, 2 = Hard

    static bool initialized = false;

    static sf::Font font("assets/arial.ttf");
    static sf::Text title(font);
    static sf::Text back_text(font);
    static sf::RectangleShape back_button({0, 0});

    // Difficulty bullets + labels
    static std::vector<sf::CircleShape> bullets(3);
    static std::vector<sf::Text> labels;

    // Reset button
    static sf::RectangleShape reset_button;
    static sf::Text reset_text(font);

    if (!initialized) {
        // === Title ===
        title.setString("SETTINGS");
        title.setCharacterSize(60);
        title.setFillColor(sf::Color(0, 200, 255));
        title.setPosition({(window.getSize().x - title.getLocalBounds().size.x) / 2.f, 150.f});

        // === Back button ===

        back_text.setString("< BACK");
        back_text.setCharacterSize(28);
        back_text.setFillColor(sf::Color::White);
        back_text.setPosition({20.f, 20.f});

        back_button.setSize({back_text.getGlobalBounds().size.x, back_text.getGlobalBounds().size.y + 20});
        back_button.setFillColor(sf::Color(10, 10, 40, 200));
        back_button.setOutlineThickness(4);
        back_button.setOutlineColor(sf::Color(0, 200, 255, 150));
        back_button.setPosition(back_text.getPosition());

        std::vector<std::string> options = {"EASY", "NORMAL", "HARD"};
        float spacing = 200.f;  // distance between bullets
        float y = 300.f;        // vertical position for bullets
        int n = options.size();

        // total width of the whole row
        float total_width = (n - 1) * spacing;
        float start_x = window.getSize().x / 2.f - total_width / 2.f;

        for (int i = 0; i < 3; i++) {
            bullets[i].setRadius(15.f);
            bullets[i].setOutlineThickness(3);
            bullets[i].setOutlineColor(sf::Color(0, 200, 255, 180));
            bullets[i].setFillColor(sf::Color::Transparent);

            // center origin
            bullets[i].setOrigin({bullets[i].getRadius(), bullets[i].getRadius()});

            // position correctly
            float bullet_x = start_x + i * spacing;
            float bullet_y = y;
            bullets[i].setPosition({bullet_x, bullet_y});

            sf::Text label(font);
            label.setString(options[i]);
            label.setCharacterSize(28);
            label.setFillColor(sf::Color::White);
            label.setPosition({
                bullets[i].getPosition().x - label.getGlobalBounds().size.x / 2.f,
                    bullets[i].getPosition().y + 50
            });
            labels.push_back(label);
        }

        // === Reset Highscore Button ===
        float button_width = 400.f;
        float button_height = 70.f;
        reset_button.setSize({button_width, button_height});
        reset_button.setFillColor(sf::Color(10, 10, 40, 200));
        reset_button.setOutlineThickness(4);
        reset_button.setOutlineColor(sf::Color(0, 200, 255, 150));
        reset_button.setPosition({
            (window.getSize().x - button_width) / 2.f,
            bullets[0].getPosition().y + 150
        });

        reset_text.setString("RESET HIGHSCORE");
        reset_text.setCharacterSize(28);
        reset_text.setFillColor(sf::Color::White);
        reset_text.setPosition({
            reset_button.getPosition().x + (button_width - reset_text.getLocalBounds().size.x) / 2.f,
            reset_button.getPosition().y + (button_height - reset_text.getLocalBounds().size.y) / 2.f - 8.f
        });

        initialized = true;
    }

    // === Draw ===
    window.clear(sf::Color(5, 5, 30));
    window.draw(title);
    window.draw(back_button);
    window.draw(back_text);

    // Update bullet fill states
    for (int i = 0; i < 3; i++) {
        if (i == difficulty) {
            bullets[i].setFillColor(sf::Color(0, 200, 255)); // filled
        } 
        else {
            bullets[i].setFillColor(sf::Color::Transparent);
        }
        window.draw(bullets[i]);
        window.draw(labels[i]);
    }

    // Draw reset button
    window.draw(reset_button);
    window.draw(reset_text);

    // === Handle input ===
    // Back button
    if (back_button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos))) {
        back_button.setOutlineColor(sf::Color(0, 255, 180));
        if (mouse_pressed){
            return 'b'; // back
        } 
    } else {
        back_button.setOutlineColor(sf::Color(0, 200, 255, 150));
    }

    // Difficulty bullets
    for (int i = 0; i < 3; i++) {
        if (bullets[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos))) {
            bullets[i].setOutlineColor(sf::Color(0, 255, 180));
            if (mouse_pressed) {
                difficulty = i; // update difficulty
            }
        } else {
            bullets[i].setOutlineColor(sf::Color(0, 200, 255, 180));
        }
    }

    // Reset highscore
    if (reset_button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos))) {
        reset_button.setOutlineColor(sf::Color(0, 255, 180));
        if (mouse_pressed) {
            highscore = 0;
            saveHighscore(highscore); // reuse your function
        }
    } else {
        reset_button.setOutlineColor(sf::Color(0, 200, 255, 150));
    }

    return -1; // nothing pressed
}

