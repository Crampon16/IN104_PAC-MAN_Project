//
//  boss_handler.hpp
//  Pac-man
//
//  Created by Liam Rampon on 30/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef boss_handler_hpp
#define boss_handler_hpp

#include <stdio.h>
#include <iostream>
#include <SDL_mixer.h>

#include "texture_wrapper.hpp"
#include "time_wrapper.hpp"

#include "entity_rebirth.hpp"
#include "stage_reborn.hpp"

const int TRANSLATION_PHASE_DURATION = 4500, ROTATION_PHASE_DURATION = 4500;

void boss_fight(SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font, std::vector<Mix_Chunk*> const &sounds, std::vector<Mix_Music*> const& tracks, bool& victory, bool& quit);

void animation(SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font, bool& quit);

void move(Uint32 delta, Avatar& pac, char input, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, PongRacket& r1, PongRacket& r2);

void handle_collisions(Avatar& pac, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, std::vector<Mix_Chunk*> const &sounds);

void display_stage(SDL_Renderer* renderer, Avatar& pac, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, PongRacket& r1, PongRacket& r2, std::vector<LTexture*> const &textures, LBitmapFont& font);

#endif /* boss_handler_hpp */
