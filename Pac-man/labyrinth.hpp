//
//  labyrinth.hpp
//  Pac-man
//
//  Created by Liam Rampon on 03/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef labyrinth_hpp
#define labyrinth_hpp

#include <stdio.h>
#include <unordered_map>
#include <string>
#include <vector>


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
    std::unordered_map<int, std::unordered_map<int, Square>> matrix;
    
    //Allows pathfinding functions to quickly locate the position of other entities
    std::vector<std::pair<int, int>> entities_positions;
};
#endif /* labyrinth_hpp */
