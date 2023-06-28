#include <array>
#include <chrono>
#include <random>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "DrawText.h"
#include "Global.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Ufo.h"
#include "Player.h"
#include "Bullet.h"
#include "Powerup.h"


int main()
{
    int start = 1;
    bool gameover = 0;
    bool nextlevel = 0;

    //The current level.
    unsigned short level = 0;
    unsigned short nextlevel_timer = NEXT_LEVEL_TRANSITION;

    //We'll use this to make the game frame rate independent.
    std::chrono::microseconds delay(0);
    std::chrono::steady_clock::time_point previous_time;
    //generate random numbers.
    std::mt19937_64 randomness(std::chrono::system_clock::now().time_since_epoch().count());

    sf::Event event;

    sf::RenderWindow window(sf::VideoMode(screen_resize * SCREEN_WIDTH, screen_resize * SCREEN_HEIGHT), "Space Invaders", sf::Style::Close);
    //Resizing the screen.
    window.setView(sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT)));
    

    //setting splash screen
    sf::Sprite splash_screen_sprite;
    sf::Texture splash_screen_texture;
    if (!splash_screen_texture.loadFromFile("/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/invaders_new/splashscreen1.jpg"))
    {
        return EXIT_FAILURE;
        
    }
    
    splash_screen_sprite.setTexture(splash_screen_texture);
    splash_screen_sprite.setScale(0.17f, 0.17f);
    
    
    //setting background
    sf::Sprite background_sprite;
    sf::Texture background_texture;
    background_texture.loadFromFile("/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/space.jpg");
    background_sprite.setScale(0.25f, 0.25f);
    background_sprite.setTexture(background_texture);
    
    
    //setting font
    sf::Texture font_texture;
    font_texture.loadFromFile("/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Font.png");

   
    EnemyManager enemy_manager;

    Player player;

    Ufo ufo(randomness);

    previous_time = std::chrono::steady_clock::now();

    while (window.isOpen())
    {
        //Making the game frame rate independent.
        std::chrono::microseconds time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time);

        delay += time;

        previous_time += time;
        
        while (FRAME_DURATION <= delay)
        {
            delay -= FRAME_DURATION;

            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                        window.close();
                }
            }
    
            if (start)
            {   window.clear();
                window.draw(splash_screen_sprite);
                window.display();
                
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    start = 0;
                }
                
              
            }
            else{

            //We're gonna show the "Game over!" text after the player's death animation.
            if (player.get_dead_animation())
            {
                gameover = 1;
            }

            if (enemy_manager.reached_player(player.get_y()))
            {
                player.die();
            }

            if (gameover == 0)
            {
                if (enemy_manager.get_enemies().size() == 0)
                {
                    if (nextlevel_timer == 0)
                    {
                        nextlevel = 0;

                        level++;
                        nextlevel_timer = NEXT_LEVEL_TRANSITION;

                        player.reset();

                        enemy_manager.reset(level);

                        ufo.reset(1, randomness);
                    }
                    //showing the next level transition.
                    else
                    {
                        nextlevel = 1;

                        nextlevel_timer--;
                    }
                }
                else
                {
                    player.update(randomness, enemy_manager.get_enemy_bullets(), enemy_manager.get_enemies(), ufo);

                    enemy_manager.update(randomness);

                    ufo.update(randomness);
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                gameover = 0;

                level = 0;

                player.reset();

                enemy_manager.reset(level);

                ufo.reset(1, randomness);
            }
            
            if (FRAME_DURATION > delay)
            {
                window.clear();

                window.draw(background_sprite);

                //When the player dies, we won't show anything but the player.
               if (0 == player.get_dead())
                {
                    enemy_manager.draw(window);

                    ufo.draw(window);
                }
                    
                player.draw(window);

                draw_text(0.25f * SIZE, 0.25f * SIZE, "Level: " + std::to_string(level), window, font_texture);

                if (gameover)
                {
                    draw_text(0.5f * (SCREEN_WIDTH - 5 * SIZE), 0.5f * (SCREEN_HEIGHT - SIZE), "Game over!", window, font_texture);
                }
                else if (nextlevel)
                {
                    draw_text(0.5f * (SCREEN_WIDTH - 5.5f * SIZE), 0.5f * (SCREEN_HEIGHT - SIZE), "Next level!", window, font_texture);
                }
                
                window.display();
            }
            }
        }
    }
}

