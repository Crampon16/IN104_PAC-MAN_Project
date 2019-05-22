//
//  pathfinding.hpp
//  Pac-man
//


#ifndef pathfinding_hpp
#define pathfinding_hpp

#include <stdio.h>

#include <queue>

#include "entity.hpp"
#include "labyrinth.hpp"

struct Stage;

//returns the shortest path between start and goal
std::stack<std::pair<int, int>> bfs(Stage& stage, std::pair<int, int> start, std::pair<int, int> goal);


//simply takes the shortest path to the player
std::stack< std::pair<int, int> > blinky_AI(int id, Stage& stage);

//heads towards the player's next node
std::stack< std::pair<int, int> > pinky_AI(int id, Stage& stage);

//heads towards the node nearest to the intersection of pac man's direction and blinky's direction
std::stack< std::pair<int, int> > inky_AI(int id, Stage& stage);

//heads towards pac-man, or go away if too close of him
std::stack< std::pair<int, int> > clyde_AI(int id, Stage& stage);



//stay still
std::stack<std::pair<int, int>> still_AI(int id, Stage& stage);

//heads towards the scatter point of the entity
std::stack<std::pair<int, int>> scatter_AI(int id, Stage& stage);

//heads towards the point at the farthest from pac-man
std::stack<std::pair<int, int>> escape_AI(int id, Stage& stage);

//heads towards the spawn point of the entity, by going through walls
std::stack<std::pair<int, int>> death_AI(int id, Stage& stage);



//get last_input and update the position
std::stack< std::pair<int, int> > pacman_AI(int id, Stage& stage);

#endif /* pathfinding_hpp */
