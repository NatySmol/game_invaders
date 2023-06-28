//
//  Animation.cpp
//  invaders
//
//  Created by Natálie Smolíková on 08.08.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//

#include "Animation.h"

#include <SFML/Graphics.hpp>


Animation::Animation(unsigned short i_animation_speed, unsigned short i_frame_width, const std::string& i_texture_location) :
    animation_iterator(0),
    animation_speed(std::max<unsigned short>(1, i_animation_speed)),
    current_frame(0),
    frame_width(i_frame_width)
{
    texture.loadFromFile(i_texture_location);

    total_frames = texture.getSize().x / frame_width;
}

//This is for the enemies.
bool Animation::change_current_frame()
{
    current_frame++;

    if (current_frame == total_frames)
    {
        current_frame = 0;

        return 1;
    }

    return 0;
}

bool Animation::update()
{
    bool output = 0;

    animation_iterator++;

    while (animation_iterator >= animation_speed)
    {
        animation_iterator -= animation_speed;
        current_frame++;

        if (current_frame == total_frames)
        {
            output = 1;

            current_frame = 0;
        }
    }

    return output;
}

void Animation::draw(short i_x, short i_y, sf::RenderWindow& i_window, const sf::Color& i_color)
{
    //colors for the explosions.
    sprite.setColor(i_color);
    sprite.setPosition(i_x, i_y);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(current_frame * frame_width, 0, frame_width, texture.getSize().y));

    i_window.draw(sprite);
}

void Animation::reset()
{
    animation_iterator = 0;
    current_frame = 0;
}
