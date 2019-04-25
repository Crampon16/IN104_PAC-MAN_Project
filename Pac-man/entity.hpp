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

enum entity_state
{
    NORMAL,
    KILLER,
    AFRAID,
    DEAD
};

class Entity
{
public:
    
    Entity(SDL_Point position, int speed, std::stack<std::pair<int,int> > path,
           std::stack<std::pair<int,int> > (*path_finder)(int, Stage&), int Id);
    
    SDL_Point get_position();
    void set_position(std::pair<int, int> square, Stage& stage);
    void set_position(SDL_Point pos, Stage& stage);
    
    void set_speed(int sp);
    
    std::pair<int, int> get_previous_square();
    
    std::vector<SDL_Rect> get_colliders();
    
    std::stack<std::pair<int, int>> get_path();
    void set_path(std::stack<std::pair<int,int>> path);
    
    entity_state state;
    
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
    
    void set_path_finding(std::stack<std::pair<int,int> > (*path_finder)(int, Stage&));
    
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
    
    std::pair<int, int> previous_square;
    
    /*
     The path finding function takes the position of the Entity on the Stage and the Stage itself. It
     empties path then fills it with the new steps to take.
     This is where Entities get different behaviours.
     */
    std::stack<std::pair<int,int> > (*path_finder)(int id, Stage&);
    
    
};
#endif /* entity_hpp */
