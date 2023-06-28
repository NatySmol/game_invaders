//
//  Player.cpp
//  invaders
//
//  Created by Natálie Smolíková on 27.07.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//


#include <array>
#include <chrono>
#include <random>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Global.h"
#include "Enemy.h"
#include "Ufo.h"
#include "Player.h"
#include "Bullet.h"
#include "Powerup.h"



Player::Player() :
    explosion(explosion_speed,SIZE,"/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Explosion.png")
{
    reset();

    bullet_texture.loadFromFile("/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/PlayerBullet.png");
    texture.loadFromFile("/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Player1.png");

    bullet_sprite.setTexture(bullet_texture);
    sprite.setTexture(texture);
}

bool Player::get_dead() const
{
    return dead;
}

bool Player::get_dead_animation() const
{
    return dead_animation;
}

unsigned char Player::get_current_power() const
{
    return current_power;
}

unsigned short Player::get_power_timer() const
{
    return power_timer;
}

unsigned short Player::get_y() const
{
    return y;
}

void Player::die()
{
    dead = 1;
}

void Player::draw(sf::RenderWindow& i_window)
{
    if (dead == 0)
    {
        sprite.setPosition(x, y);
        sprite.setTextureRect(sf::IntRect(SIZE * current_power,0, SIZE, SIZE));

        for (const Bullet& bullet : bullets)
        {
            bullet_sprite.setPosition(bullet.x, bullet.y);

            i_window.draw(bullet_sprite);
        }

        i_window.draw(sprite);

        if (shield_animation_over == 0)
        {
            //Once we get hit while having a shield, the shield will be destroyed. We'll show a blue explosion.
            explosion.draw(x, y, i_window, sf::Color(0, 109, 255));
        }
    }
    else if (dead_animation == 0)
    {
        explosion.draw(x, y, i_window, sf::Color(255, 36, 0));
    }
}

void Player::reset()
{
    dead = 0;
    dead_animation = 0;
    shield_animation_over = 1;

    current_power = 0;
    reload_timer = 0;

    power_timer = 0;
    x = 0.5f * (SCREEN_WIDTH - SIZE);
    y = SCREEN_HEIGHT - 2 * SIZE;

    bullets.clear();

    explosion.reset();
}

void Player::update(std::mt19937_64& i_random_engine, std::vector<Bullet>& i_enemy_bullets, std::vector<Enemy>& i_enemies, Ufo& i_ufo)
{
    if (0 == dead)
    {
        unsigned char powerup_type;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            if (4 == current_power)
            {
                //Mirrored controls power-DOWN!
                x = std::min<int>(player_speed + x, SCREEN_WIDTH - 2 * SIZE);
            }
            else
            {
                x = std::max<int>(x - player_speed, SIZE);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            if (4 == current_power)
            {
                //Mirrored controls power-DOWN!
                x = std::max<int>(x - player_speed, SIZE);
            }
            else
            {
                x = std::min<int>(player_speed + x, SCREEN_WIDTH - 2 * SIZE);
            }
        }

        if (reload_timer == 0)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if (2 == current_power)
                {
                    //faster shooting
                    reload_timer = FAST_RELOAD_DURATION;
                }
                else
                {
                    reload_timer = RELOAD_DURATION;
                }

                bullets.push_back(Bullet(0, -player_bulletspeed, x, y));

                if (3 == current_power)
                {
                    //shootinh more bullets
                    bullets.push_back(Bullet(0, -player_bulletspeed, x - 0.375f * SIZE, y));
                    bullets.push_back(Bullet(0, -player_bulletspeed, x + 0.375f * SIZE, y));
                }
            }
        }
        else
        {
            reload_timer--;
        }

        for (Bullet& enemy_bullet : i_enemy_bullets)
        {
            if (get_hitbox().intersects(enemy_bullet.get_hitbox()))
            {
                if (1 == current_power)
                {
                    //we get a shield
                    current_power = 0;

                    shield_animation_over = 0;
                }
                else
                {
                    dead = 1;
                }

                enemy_bullet.dead = 1;

                break;
            }
        }

        powerup_type = i_ufo.check_powerup_collision(get_hitbox());

        if (0 < powerup_type)
        {
            current_power = powerup_type;

            power_timer = powerup_duration;
        }

        if (power_timer == 0)
        {
            current_power = 0;
        }
        else
        {
            power_timer--;
        }

        if (shield_animation_over == 0)
        {
            shield_animation_over = explosion.update();
        }
    }
    else if ( dead_animation == 0)
    {
        dead_animation = explosion.update();
    }

    for (Bullet& bullet : bullets)
    {
        bullet.update();
        
        if ( bullet.dead == 0)
        {
            if (i_ufo.check_bullet_collision(i_random_engine, bullet.get_hitbox()))
            {
                bullet.dead = 1;
            }
        }
    }

    for (Enemy& enemy : i_enemies)
    {
        for (Bullet& bullet : bullets)
        {
            if (0 == bullet.dead && 0 < enemy.get_health() && 1 == enemy.get_hitbox().intersects(bullet.get_hitbox()))
            {
                bullet.dead = 1;

                enemy.hit();

                break;
            }
        }
    }

    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& i_bullet)
    {
        return  i_bullet.dead == 1;
    }), bullets.end());
}

sf::IntRect Player::get_hitbox() const
{
    return sf::IntRect(x + 0.125f * SIZE, y + 0.125f * SIZE, 0.75f * SIZE, 0.75f * SIZE);
}
