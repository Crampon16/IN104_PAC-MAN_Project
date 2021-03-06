//
//  entity.hpp
//  Pac-man
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

struct Precise_Point
{
    float x = 0, y = 0;
    void operator=(SDL_Point point)
    {
        x = point.x;
        y = point.y;
    }
};

SDL_Point to_SDL(Precise_Point point);

class Entity
{
public:
    
    Entity(SDL_Point position, int speed, std::stack<std::pair<int,int> > path,
           std::stack<std::pair<int,int> > (*path_finder)(int, Stage&), int Id);
    
    SDL_Point get_position();
    std::pair<int,int> get_square_position();
    
    void set_position(std::pair<int, int> square, Stage& stage);
    void set_position(SDL_Point pos, Stage& stage);
    
    void set_speed(int sp);
    
    void set_previous_square(std::pair<int, int> square);
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
    std::stack<std::pair<int, int>>(*get_path_finding())(int,Stage&);
    
private:
    
    Precise_Point position;
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
