//
//  boss_handler.hpp
//  Pac-man
//


#ifndef boss_handler_hpp
#define boss_handler_hpp

#include <stdio.h>
#include <iostream>

#include "texture_wrapper.hpp"
#include "time_wrapper.hpp"

#include "entity_rebirth.hpp"
#include "stage_reborn.hpp"

const int TRANSLATION_PHASE_DURATION = 2000, ROTATION_PHASE_DURATION = 2000;

void boss_fight(SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font);

bool animation(SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font);

void move(Uint32 delta, Avatar& pac, char input, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, PongRacket& r1, PongRacket& r2);

void handle_collisions(Avatar& pac, MergeHead& mhead, TetrisGrid& grid, PongBall& ball);

void display_stage(SDL_Renderer* renderer, Avatar& pac, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, PongRacket& r1, PongRacket& r2, std::vector<Pellet> pellets, std::vector<LTexture*> const &textures, LBitmapFont& font);

#endif /* boss_handler_hpp */
