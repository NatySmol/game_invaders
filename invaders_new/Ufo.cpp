//
//  Ufo.cpp
//  invaders
//
//  Created by Natálie Smolíková on 27.07.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//

#include <array>
#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Global.h"
#include "Ufo.h"
#include "Powerup.h"

Ufo::Ufo(std::mt19937_64& i_random_engine) :
    y(SIZE),
    powerup_distribution(0, powerup_types - 1),
    timer_distribution(ufo_timer_min, ufo_timer_max),
    animation(ufo_animationspeed, 2 * SIZE, "/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Ufo.png"),
    explosion(explosion_speed, 2 * SIZE, "/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/ExplosionBig.png")
{
    reset(1, i_random_engine);
    
    powerup_animations.push_back(Animation(powerup_animationspeed, SIZE, "/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Powerup0.png"));
    powerup_animations.push_back(Animation(powerup_animationspeed, SIZE, "/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Powerup1.png"));
    powerup_animations.push_back(Animation(powerup_animationspeed, SIZE, "/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Powerup2.png"));
    powerup_animations.push_back(Animation(powerup_animationspeed, SIZE, "/Users/nataliesmolikova/Documents/MFF_UK/invaders_new/Powerup3.png"));
}
    


bool Ufo::check_bullet_collision(std::mt19937_64& i_random_engine, const sf::IntRect& i_bullet_hitbox)
{
    if (dead == 0)
    {
        if (1 == get_hitbox().intersects(i_bullet_hitbox))
        {
            dead = 1;

            explosion_x = x;

            powerups.push_back(Powerup(x + 0.5f * SIZE, y, powerup_distribution(i_random_engine)));

            return 1;
        }
    }
    
    return 0;
}

unsigned char Ufo::check_powerup_collision(const sf::IntRect& i_player_hitbox)
{
    for (Powerup& powerup : powerups)
    {
        if (0 == powerup.dead && 1 == powerup.get_hitbox().intersects(i_player_hitbox))
        {
            powerup.dead = 1;

            //Plus 1, because 0 means we didn't pick up any powerups.
            return 1 + powerup.type;
        }
    }
    
    return 0;
}

void Ufo::draw(sf::RenderWindow& i_window)
{
    if (dead == 0)
    {
        animation.draw(x, y, i_window);
    }

    if (dead_animation_over == 0)
    {
        explosion.draw(explosion_x, y - 0.5f * SIZE, i_window, sf::Color(255, 36, 0));
    }

    for (Powerup& powerup : powerups)
    {
        powerup_animations[powerup.type].draw(powerup.x, powerup.y, i_window);
    }
}

void Ufo::reset(bool i_dead, std::mt19937_64& i_random_engine)
{
    dead = i_dead;
    dead_animation_over = 0;

    explosion_x = SCREEN_WIDTH;
    x = SCREEN_WIDTH;

    timer = timer_distribution(i_random_engine);

    powerups.clear();

    animation.reset();
    explosion.reset();
}

void Ufo::update(std::mt19937_64& i_random_engine)
{
    if (dead == 0)
    {
        x -= ufo_speed;

        //As soon as the UFO leaves the screen, it gets destroyed. But no powerups will appear.
        if (x <= -2 * SIZE)
        {
            dead = 1;
        }

        animation.update();
    }
    else
    {
        if (dead_animation_over == 0)
        {
            dead_animation_over = explosion.update();
        }

        if (timer == 0)
        {
            reset(0, i_random_engine);
        }
        else
        {
            timer--;
        }
    }

    for (Powerup& powerup : powerups)
    {
        powerup.y += powerup_speed;

        if (SCREEN_HEIGHT <= powerup.y)
        {
            powerup.dead = 1;
        }
    }

    for (Animation& powerup_animation : powerup_animations)
    {
        powerup_animation.update();
    }

    powerups.erase(remove_if(powerups.begin(), powerups.end(), [](const Powerup& i_powerup)
    {
        return 1 == i_powerup.dead;
    }), powerups.end());
}

sf::IntRect Ufo::get_hitbox() const
{
    return sf::IntRect(x, y, 2 * SIZE, SIZE);
}



