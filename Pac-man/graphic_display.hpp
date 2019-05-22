//
//  graphic_display.hpp
//  Pac-man
//


#ifndef graphic_display_hpp
#define graphic_display_hpp

#include <stdio.h>
#include <string>

#include "SDL2/SDL.h"

#include "constants.hpp"
#include "labyrinth.hpp"

const int AFRAID_ID = 5, BLINKING_ID = 6, DEAD_ID = 7, WALL_ID = 8;

void display(SDL_Renderer* renderer, Stage stage, LBitmapFont& font, std::vector<LTexture*> const &textures);

#endif /* graphic_display_hpp */
