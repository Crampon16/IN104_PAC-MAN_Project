//
//  graphic_display.hpp
//  Pac-man
//
//  Created by Liam Rampon on 03/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef graphic_display_hpp
#define graphic_display_hpp

#include <stdio.h>

#include "SDL2/SDL.h"

#include "constants.hpp"
#include "labyrinth.hpp"

void display(SDL_Renderer* renderer, Stage stage);

#endif /* graphic_display_hpp */
