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

class Entity;

//A Stage is composed of a matrix of Squares
struct Square
{
    bool obstructed = false;
    bool is_node = false;
    
    bool is_tunnel = false;
    int tunnel_target_line = -1;
    int tunnel_target_column = -1;
    
    std::string item = "";
    
    std::vector<std::string> entities;
};

struct Stage
{
    std::vector<std::vector<Square>> matrix;
    
    //Allows pathfinding functions to quickly locate the position of other entities
    std::vector<std::pair<int, int>> entities_positions;
    
    std::vector<Entity> entities;
    
    std::string last_input;
};

Stage init_stage(std::string path);
#endif /* labyrinth_hpp */
