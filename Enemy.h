//
//  Enemy.h
//  invaders
//
//  Created by Natálie Smolíková on 29.07.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//

#pragma once
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Bullet.h"

class Enemy
{
    //-1 - Left
    //0 - Down
    //1 - Right
    int direction;

    unsigned char health;
    //The enemy will appear yellow for a few frames after being hit, so that the player knows about it.
    unsigned char hit_timer;
    //0 - orange one
    //1 - blue one
    //2 - purple one
    unsigned char type;

    unsigned short x;
    unsigned short y;
public:
    Enemy(unsigned char i_type, unsigned short i_x, unsigned short i_y);

    unsigned char get_health() const;
    unsigned char get_hit_timer() const;
    unsigned char get_type() const;

    unsigned short get_x() const;
    unsigned short get_y() const;

    void hit();
    void move();
    void shoot(std::vector<Bullet>& i_enemy_bullets);
    void update();
    void move_down();

    sf::IntRect get_hitbox() const;
};
