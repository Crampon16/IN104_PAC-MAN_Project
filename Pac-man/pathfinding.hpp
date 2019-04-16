//
//  pathfinding.hpp
//  Pac-man
//
//  Created by Liam Rampon on 09/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef pathfinding_hpp
#define pathfinding_hpp

#include <stdio.h>

#include "entity.hpp"
#include "labyrinth.hpp"

//symply takes the shortest path to the player
std::stack< std::pair<int, int> > blinky_AI(std::pair<int, int> pos, Stage& stage);

//heads towards the player's next node
std::stack< std::pair<int, int> > pinky_AI(std::pair<int, int> pos, Stage& stage);

//heads towards the node nearest to the intersection of pac man's direction and blinky's direction
std::stack< std::pair<int, int> > inky_AI(std::pair<int, int> pos, Stage& stage);

//heads towards pac-man, or go away if too close of him
std::stack< std::pair<int, int> > clyde_AI(std::pair<int, int> pos, Stage& stage);


#endif /* pathfinding_hpp */
