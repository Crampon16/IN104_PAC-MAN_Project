//
//  level_handler.hpp
//  Pac-man
//
//  Created by Liam Rampon on 25/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef level_handler_hpp
#define level_handler_hpp

#include <stdio.h>

#include <string>
#include <vector>

#include "texture_wrapper.hpp"
#include "time_wrapper.hpp"
#include "labyrinth.hpp"
#include "controller.hpp"
#include "graphic_display.hpp"

bool classic_level(std::string layout, SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font);

/*
 This function detects several type of collisions and applies different effects to each:
 * gum/pac collisions destroy the gum and increase the score
 * super gum/pac collisions destroy the gum and and turn on killer mode for some time
 * ghost/pac collisions (out of killer mode) makes pac respawn and decreases the amount of lives remaining
 * ghost/pac collisions (in killer mode) increases the score and set ghost to their back to spawn behaviour
 */
void handle_collisions(Stage& stage);
void handle_AIs(Stage& stage);
#endif /* level_handler_hpp */
