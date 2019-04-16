//
//  entity.cpp
//  Pac-man
//
//  Created by Liam Rampon on 02/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "entity.hpp"

Entity::Entity(SDL_Point pos, int spd, std::stack<std::pair<int,int> > paff, std::stack<std::pair<int,int> > (*paff_finder)(std::pair<int, int>, Stage&), int ID)
{
    position = pos;
    speed = spd;
    path = paff;
    if (path.empty())
    {
        path.push({pos.x/STAGE_WIDTH, pos.y/STAGE_HEIGHT});
    }
    path_finder = paff_finder;
    Id = ID;
}

SDL_Point Entity::get_position()
{
    return position;
}

std::vector<SDL_Rect> Entity::get_colliders()
{
    return colliders;
}

float sign(float n)
{
    if (n>0) {return 1;}
    else if (n<0) {return -1;}
    else {return 0;}
}

std::pair<int,int> get_square_position(SDL_Point position)
{
    std::pair<int, int> square;
    square.first = (position.y/STAGE_HEIGHT);
    square.second = (position.x/STAGE_WIDTH);
    return square;
}

void Entity::move(int delta, Stage& stage)
{
    std::pair<int, int> square = get_square_position(position);
    
    SDL_Point target_position;
    std::pair<int,int> target_square = path.top();
    
    target_position.x = target_square.second*STAGE_HEIGHT + SQUARE_SIZE/2;
    target_position.y = target_square.first*STAGE_WIDTH + SQUARE_SIZE/2;
    
    //in this case the entity is stationnary
    if(target_position.x == position.x and target_position.y == position.y)
    {
        path.pop();
        if (path.empty())
            find_path(stage);
    }
    else
    {
        SDL_Point direction;
        direction.x = target_position.x - position.x;
        direction.y = target_position.y - position.y;
        
        float norm = sqrt(direction.x*direction.x + direction.y*direction.y);
        
        //if the movement would reach beyond the target
        if (norm <= speed*delta)
        {
            position = target_position;
            path.pop();
            if (path.empty())
            {
                find_path(stage);
            }
        }
        else
        {
            position.x += direction.x*speed*delta/norm;
            position.y += direction.y*speed*delta/norm;
        }
        
        square = get_square_position(position);
        stage.entities_positions[Id] = square;
    }
    
}

void Entity::find_path(Stage& stage)
{
    std::pair<int, int> square;
    square.first = position.y % STAGE_HEIGHT;
    square.second = position.x % STAGE_WIDTH;
    
    path = path_finder(square, stage);
}

void Entity::set_path_finding(std::stack<std::pair<int,int> > (*paff_finder)(std::pair<int, int>, Stage&))
{
    path_finder = paff_finder;
}
