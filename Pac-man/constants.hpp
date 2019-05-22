//
//  constants.h
//  Pac-man
//
//  Created by Liam Rampon on 03/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef constants_h
#define constants_h

//in lines and columns
const int STAGE_WIDTH = 28, STAGE_HEIGHT = 31;

//in pixels
const int SQUARE_SIZE = 20;

//in pixels
const int SCREEN_WIDTH = SQUARE_SIZE*STAGE_WIDTH;
const int SCREEN_HEIGHT = SQUARE_SIZE*STAGE_HEIGHT + 4*SQUARE_SIZE;

//boss related constants
const int BOSS_STAGE_WIDTH = SQUARE_SIZE*STAGE_WIDTH;
const int BOSS_STAGE_HEIGHT = SQUARE_SIZE*STAGE_HEIGHT;
const int AVATAR_SIZE = SQUARE_SIZE;

const int pac_man_id = 0;

const int clyde_chicken_distance = 4;
const int new_game_music_time = 4250;

#endif /* constants_h */
