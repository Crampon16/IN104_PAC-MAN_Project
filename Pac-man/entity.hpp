//
//  entity.hpp
//  Pac-man
//
//  Created by Liam Rampon on 02/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef entity_hpp
#define entity_hpp

#include <stdio.h>
#include <vector>
#include <map>
#include <stack>
#include <math.h>

#include "SDL2/SDL.h"

#include "labyrinth.hpp"
#include "texture_wrapper.hpp"
#include "constants.hpp"

struct Stage;

class Entity
{
public:
    
    Entity(SDL_Point position, int speed, std::stack<std::pair<int,int> > path,
           std::stack<std::pair<int,int> > (*path_finder)(std::pair<int, int>, Stage&), int Id);
    
    SDL_Point get_position();
    std::vector<SDL_Rect> get_colliders();
    std::pair<int, int> get_current_direction();
    
    /*
     Updates position: "postion += delta*current_speed"
     delta is measured in milliseconds.
     If the top of path is reached, pops it.
     If path is empty, calls find_path().
     */
    void move(int delta, Stage& stage);
    
    /*
     Calls path_finder() to find what new path is to be taken, then changes path and current_speed
     to do so.
     */
    void find_path(Stage& stage);
    
    //Yeah that's ugly.
    void set_path_finding(std::stack<std::pair<int,int> > (*path_finder)(std::pair<int, int>, Stage&));
    
private:
    
    SDL_Point position;
    //contains every hitbox of the entity
    std::vector<SDL_Rect> colliders;
    
    //how fast the Entity moves
    int speed;
    
    //Entity's Id
    int Id;
    
    //What Squares the Entity is going to go through.
    /*
     Once a square is reached, it is removed from the stack. Once the stack is empty, find_path() is called.
     */
    std::stack<std::pair<int,int> > path;
    
    /*
     The path finding function takes the position of the Entity on the Stage and the Stage itself. It
     empties path then fills it with the new steps to take.
     This is where Entities get different behaviours.
     */
    //TODO: ask Liam to explain this syntax
    std::stack<std::pair<int,int> > (*path_finder)(std::pair<int, int>, Stage&);
    
    
};
#endif /* entity_hpp */
