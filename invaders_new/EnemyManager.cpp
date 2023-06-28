//
//  EnemyManager.cpp
//  invaders
//
//  Created by Natálie Smolíková on 27.07.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//

#include <array>
#include <chrono>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>


#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Global.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Bullet.h"
using std::cerr;
using std::cout;

FILE* soubor;
int znak;

string read_level(FILE* file)
{
    string level_scheme = "";
    znak = fgetc(file);
    while (znak != 46)
    {
        level_scheme += znak;

        znak = fgetc(file);
    }
            
    return level_scheme;
}

string load_level(unsigned short i_level)
{
    string level_scheme ="";
    soubor = fopen(("/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/levels/level"+to_string(i_level)+".txt").c_str(),"r");
    level_scheme = read_level(soubor);
    fclose(soubor);
    return level_scheme;

}


EnemyManager::EnemyManager() :
    shoot_distribution(0, enemy_shoot_chance)
{
    reset(0);

    enemy_bullet_texture.loadFromFile("/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/EnemyBullet.png");

    enemy_bullet_sprite.setTexture(enemy_bullet_texture);

    enemy_animations.push_back(Animation(1 + move_pause,SIZE,"/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Enemy0.png"));
    enemy_animations.push_back(Animation(1 + move_pause, SIZE,"/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Enemy1.png"));
    enemy_animations.push_back(Animation(1 + move_pause, SIZE,"/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Enemy2.png"));
    
    
}

bool EnemyManager::reached_player(unsigned short i_player_y) const
{
    for (const Enemy& enemy : enemies)
    {
        if (enemy.get_y() > i_player_y - 0.5f * SIZE)
        {
            //As soon as the enemies reach the player, the game is over!
            return 1;
        }
    }

    return 0;
}

void EnemyManager::draw(sf::RenderWindow& i_window)
{
    for (const Bullet& bullet : enemy_bullets)
    {
     
        //Drawing the bullet
        enemy_bullet_sprite.setPosition(bullet.x, bullet.y);
        enemy_bullet_sprite.setTextureRect(sf::IntRect(SIZE * bullet.previous_x.size(), 0, SIZE, SIZE));

        i_window.draw(enemy_bullet_sprite);
    }

    for (Enemy& enemy : enemies)
    {
        //When the enemy gets hit, it appears yellow
        sf::Color enemy_color = sf::Color(255, 255, 0);

        if (0 == enemy.get_hit_timer())
        {
            switch (enemy.get_type())
            {
                case 0:
                {
                    enemy_color = sf::Color(240,128,128);

                    break;
                }
                case 1:
                {
                    enemy_color = sf::Color(0,191,255);

                    break;
                }
                case 2:
                {
                    enemy_color = sf::Color(199,21,133);
                }
            }
        }

        enemy_animations[enemy.get_type()].draw(enemy.get_x(), enemy.get_y(), i_window, enemy_color);
    }
}

void EnemyManager::reset(unsigned short i_level)
{
    unsigned char enemy_x = 0;
    unsigned char enemy_y = 0;

    std::string level_scheme = "";

    move_pause = std::max<short>(enemy_movestartmin, enemy_movestart - enemy_move_decrease * i_level);
    move_timer = move_pause;

    shoot_distribution = std::uniform_int_distribution<unsigned short>(0, std::max<short>(enemy_shoot_chance_min, enemy_shoot_chance - enemy_shoot_chance_increase * i_level));

    for (Animation& enemy_animation : enemy_animations)
    {
        enemy_animation.reset();
    }

    enemy_bullets.clear();

    enemies.clear();

    //There are 8 levels. Once the player finishes level 8, we go back to level 4.
    if (number_levels <= i_level)
    {
        i_level = 0.5f * number_levels + i_level % static_cast<unsigned char>(0.5f * number_levels);
    }
    
    //loading level scheme depending on current level
    level_scheme = load_level(i_level);
    

    //Here we're converting each character into an enemy.
    for (char drawing_char : level_scheme)
    {
        enemy_x++;

        switch (drawing_char)
        {
            case '\n':
            {
                enemy_x = 0;
                enemy_y++;

                break;
            }
            case '0':
            {
                enemies.push_back(Enemy(0, SIZE * (1 + enemy_x), SIZE * (2 + enemy_y)));

                break;
            }
            case '1':
            {
                enemies.push_back(Enemy(1, SIZE * (1 + enemy_x), SIZE * (2 + enemy_y)));

                break;
            }
            case '2':
            {
                enemies.push_back(Enemy(2, SIZE * (1 + enemy_x), SIZE * (2 + enemy_y)));
            }
        }
    }
}

void EnemyManager::update(std::mt19937_64& i_random_engine)
{
    std::vector<Enemy>::iterator dead_enemies_start;

    if (0 == move_timer)
    {
        move_timer = move_pause;

        
       for (Enemy& enemy : enemies)
        {
            enemy.move();
        }
        for (Animation& enemy_animation : enemy_animations)
        {
            enemy_animation.change_current_frame();
        }
    }
    else
    {
        move_timer--;
    }

    for (Enemy& enemy : enemies)
    {
        enemy.update();

        if (0 == shoot_distribution(i_random_engine))
        {
            enemy.shoot(enemy_bullets);
        }
    }

    
    dead_enemies_start = remove_if(enemies.begin(), enemies.end(), [](const Enemy& i_enemy)
    {
        return 0 == i_enemy.get_health();
    });

    
    enemies.erase(dead_enemies_start, enemies.end());

    for (Bullet& enemy_bullet : enemy_bullets)
    {
        enemy_bullet.update();
    }

    enemy_bullets.erase(remove_if(enemy_bullets.begin(), enemy_bullets.end(), [](const Bullet& i_bullet)
    {
        return 1 == i_bullet.dead;
    }), enemy_bullets.end());
}

std::vector<Bullet>& EnemyManager::get_enemy_bullets()
{
    return enemy_bullets;
}

std::vector<Enemy>& EnemyManager::get_enemies()
{
    return enemies;
}



