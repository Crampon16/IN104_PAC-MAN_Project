//
//  controller.hpp
//  Pac-man
//
//  Created by Liam Rampon on 16/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef controller_hpp
#define controller_hpp

#include <stdio.h>

#include "SDL2/SDL.h"

#include "labyrinth.hpp"

//This function changes the last registered input in the stage to the one in e
void get_keyboard_input(Stage& stage, SDL_Event e);

#endif /* controller_hpp */
