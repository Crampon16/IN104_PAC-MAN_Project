//
//  controller.cpp
//  Pac-man
//
//  Created by Liam Rampon on 16/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "controller.hpp"

using namespace std;

void get_keyboard_input(Stage& stage, SDL_Event e)
{
    if (e.type == SDL_KEYDOWN and e.key.repeat == false)
    {
        stage.last_key_input = e.key.keysym.sym;
        //cout << stage.last_key_input << endl;
    }
}

