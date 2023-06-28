//
//  Player.h
//  invaders
//
//  Created by Natálie Smolíková on 29.07.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//
#pragma once
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>


class Player
{
    bool dead;
    bool dead_animation;
    bool shield_animation_over;

    unsigned char current_power;
    unsigned char reload_timer;

    unsigned short power_timer;
    unsigned short x;
    unsigned short y;

    std::vector<Bullet> bullets;

    sf::Sprite bullet_sprite;
    sf::Sprite sprite;

    sf::Texture bullet_texture;
    sf::Texture texture;

    Animation explosion;
public:
    Player();

    bool get_dead() const;
    bool get_dead_animation() const;

    unsigned char get_current_power() const;

    unsigned short get_power_timer() const;
    unsigned short get_y() const;

    void die();
    void draw(sf::RenderWindow& i_window);
    void reset();
    void update(std::mt19937_64& i_randomness, std::vector<Bullet>& i_enemy_bullets, std::vector<Enemy>& i_enemies, Ufo& i_ufo);

    sf::IntRect get_hitbox() const;
};
