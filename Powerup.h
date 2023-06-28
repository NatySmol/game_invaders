//
//  Powerup.h
//  invaders_new
//
//  Created by Natálie Smolíková on 29.07.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//
#pragma once
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>

struct Powerup
{
    bool dead;

    short x;
    short y;

    //0 - Shield
    //1 - Fast reload
    //2 - 3 bullets
    //3 - Mirrored controls (power-DOWN! I'm so proud of this joke)
    //Damn, I should've used enums. That would've made the code more readable.
    unsigned char type;

    Powerup(short i_x, short i_y, unsigned char i_type) :
        dead(0),
        x(i_x),
        y(i_y),
        type(i_type)
    {
        
    }

    sf::IntRect get_hitbox() const
    {
        return sf::IntRect(x + 0.25f * SIZE, y + 0.25f * SIZE, 0.5f * SIZE, 0.5f * SIZE);
    }
};
