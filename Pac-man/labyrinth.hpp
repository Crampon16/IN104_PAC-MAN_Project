//
//  labyrinth.hpp
//  Pac-man
//
//  Created by Liam Rampon on 03/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef labyrinth_hpp
#define labyrinth_hpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include <unordered_map>
#include <string>
#include <vector>

#include "constants.hpp"
#include "entity.hpp"
#include "pathfinding.hpp"

class Entity;

//A Stage is composed of a matrix of Squares
struct Square
{
    bool obstructed = false;
    bool is_node = false;
    bool go_up = false;
    
    bool is_tunnel = false;
    int tunnel_target_line = -1;
    int tunnel_target_column = -1;
    
    std::string item = "";
    
    std::vector<std::string> entities;
};

struct Stage
{
    std::vector<std::vector<Square>> matrix;
    
    //Allows pathfinding functions to quickly locate some positions
    std::vector<std::pair<int, int>> entities_positions;
    std::vector<std::pair<int, int>> entities_spawn_point;
    std::vector<std::pair<int, int>> entities_spawn_direction;
    std::vector< std::stack<std::pair<int, int>> (*)(int, Stage&)> normal_pathfinder;
    std::vector<Entity> entities;
    
    //these variables characterize what state the game is in
    char last_key_input;
    
    Uint32 killer_mode_start;
    
    int lives = 3;
    
    int score = 0;
    int number_of_gums = 0;
};

Stage init_stage(std::string path);

#endif /* labyrinth_hpp */
