//
//  DrawText.h
//  invaders
//
//  Created by Natálie Smolíková on 29.07.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//

#pragma once
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>



void draw_text(short i_x, short i_y, const std::string& i_text, sf::RenderWindow& i_window, const sf::Texture& i_font_texture);
