//
//  Global.h
//  invaders
//
//  Created by Natálie Smolíková on 29.07.2022.
//  Copyright © 2022 Natálie Smolíková. All rights reserved.
//

#pragma once
#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>



#include <cmath>
using namespace std;


//GENERAL
constexpr unsigned char SIZE = 16;
constexpr unsigned short SCREEN_HEIGHT = 180;
constexpr unsigned short SCREEN_WIDTH = 320;
//Same logic with ENEMY_MOVE_PAUSE_START.
constexpr unsigned char FAST_RELOAD_DURATION = 7;
//The transition to the next level will last 64 frames or about 1 second.
constexpr unsigned char NEXT_LEVEL_TRANSITION = 64;
constexpr unsigned char RELOAD_DURATION = 31;
constexpr unsigned char screen_resize = 4;
constexpr unsigned char number_levels = 8;



//ENEMY
constexpr unsigned char enemy_bulletspeed = 2;
constexpr unsigned char enemy_hittimer = 2;
//The more enemies we kill, the faster they become.
constexpr unsigned char enemy_move_decrease = 1;
//Setting this to 1 or even 0 results in terrible enemy movement. So don't do it.
//...
//...
//You're gonna do it anyway, aren't you?
constexpr unsigned char enemy_move_min = 3;
//"Why not 64?" - you may ask. Well, it's because if we set it to 0, they will still have 1 frame pause. So 63 + 1 = 64.
constexpr unsigned char enemy_movestart = 63;
constexpr unsigned char enemy_movestartmin = 47;
constexpr unsigned char enemy_speed = 5;
constexpr unsigned char enemy_types = 3;
constexpr unsigned char explosion_speed = 2;
constexpr unsigned short enemy_shoot_chance = 4096;
constexpr unsigned short enemy_shoot_chance_increase = 64;
constexpr unsigned short enemy_shoot_chance_min = 1024;


//PLAYER
constexpr unsigned char player_bulletspeed = 4;
constexpr unsigned char player_speed = 2;


//POWER-UP
constexpr unsigned char powerup_animationspeed = 16;
constexpr unsigned char powerup_speed = 2;
constexpr unsigned char powerup_types = 4;
constexpr unsigned short powerup_duration = 512;


//UFO
constexpr unsigned char ufo_animationspeed = 8;
constexpr unsigned char ufo_speed = 1;
constexpr unsigned short ufo_timer_max = 1024;
constexpr unsigned short ufo_timer_min = 768;


constexpr std::chrono::microseconds FRAME_DURATION(16667);

