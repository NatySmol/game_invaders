//
//  Enemy.cpp
//  invaders
//
//  Created by Natálie Smolíková on 27.07.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//

#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

#include "Global.h"
#include "Enemy.h"
#include "Animation.h"
#include "Bullet.h"

using namespace std;

Enemy::Enemy(unsigned char i_type, unsigned short i_x, unsigned short i_y) :
    direction(0 == (i_y / SIZE) % 2 ? -1 : 1),
    health(1 + i_type),
    hit_timer(0),
    type(i_type),
    x(i_x),
    y(i_y)
{
    
}

unsigned char Enemy::get_health() const
{
    return health;
}

unsigned char Enemy::get_hit_timer() const
{
    return hit_timer;
}

unsigned char Enemy::get_type() const
{
    return type;
}

unsigned short Enemy::get_x() const
{
    return x;
}

unsigned short Enemy::get_y() const
{
    return y;
}

void Enemy::hit()
{
    hit_timer = enemy_hittimer;
}
bool return_value = 0;

void Enemy::move()
{
    if (direction != 0)
    {
        if ((direction==1 && x == SCREEN_WIDTH - 2 * SIZE)|| (direction == -1 && x == SIZE))
        {
            //Once we reach the edge, we start moving down until we reach the next row.
            direction = 0;

            y += enemy_speed;
        }
        else
        {
            x = std::clamp(x + 5 * direction, 16, 320 - 2 * 16);
            
        }
    }
    
    else
    {
        y = std::min<short>(y + enemy_speed, SIZE * ceil(y / static_cast<float>(SIZE)));

        if (y == SIZE * ceil(y / static_cast<float>(SIZE)))
        {
            //Moving in a snake pattern. We use the modulo operator to decide whether to move left or right.
            direction = 0 == (y / SIZE) % 2 ? -1 : 1;
        }
    }
}
    


void Enemy::shoot(std::vector<Bullet>& i_enemy_bullets)
{
    switch (type)
    {
        case 0:
        {
            i_enemy_bullets.push_back(Bullet(0, enemy_bulletspeed, x, y));

            break;
        }
        case 1:
        {
            i_enemy_bullets.push_back(Bullet(0.125f * enemy_bulletspeed, enemy_bulletspeed, x, y));
            i_enemy_bullets.push_back(Bullet(-0.125f * enemy_bulletspeed, enemy_bulletspeed, x, y));

            break;
        }
        case 2:
        {
            i_enemy_bullets.push_back(Bullet(0, enemy_bulletspeed, x, y));
            i_enemy_bullets.push_back(Bullet(0.25f * enemy_bulletspeed, enemy_bulletspeed, x, y));
            i_enemy_bullets.push_back(Bullet(-0.25f * enemy_bulletspeed, enemy_bulletspeed, x, y));
        }
    }
}

void Enemy::update()
{
    if (0 < hit_timer)
    {
        if (1 == hit_timer)
        {
            health = std::max(0, health - 1);
        }

        hit_timer--;
    }
}

sf::IntRect Enemy::get_hitbox() const
{
    return sf::IntRect(x + 0.25f * SIZE, y + 0.25f * SIZE, 0.5f * SIZE, 0.5f * SIZE);
}
