//
//  entity.cpp
//  Pac-man
//
//  Created by Liam Rampon on 02/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "entity.hpp"

Entity::Entity(SDL_Point pos, SDL_Point spd, SDL_Point current_spd, std::stack<std::pair<int,int> > paff, std::stack<std::pair<int,int> > (*paff_finder)(std::pair<int, int>, Stage&), int ID)
{
    position = pos;
    speed = spd;
    current_speed = current_spd;
    path = paff;
    path_finder = paff_finder;
    Id = ID;
}

SDL_Point Entity::get_postion()
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

void Entity::move(int delta, Stage& stage)
{
    SDL_Point former_position = position;

    position.x = position.x + delta*current_speed.x;
    position.y = position.y + delta*current_speed.y;
    //delta is measured in milliseconds.

    std::pair<int, int> square;
    square.first = position.y % STAGE_HEIGHT;
    square.second = position.x % STAGE_WIDTH;

    SDL_Point target_position;
    std::pair<int,int> target_square = path.top();

    target_position.x = target_square.second*STAGE_HEIGHT + SQUARE_SIZE/2;
    target_position.y = target_square.first*STAGE_WIDTH + SQUARE_SIZE/2;

    if ((target_position.x - former_position.x)*(target_position.x - position.x) + (target_position.y - former_position.y)*(target_position.y - position.y) <= 0)
    {
        if (square != path.top())
        {
            //error (unit test)
        }
        SDL_Point error;
        error.x = (target_position.x - position.x)*sign(target_position.x - position.x);
        error.y = (target_position.y - position.y)*sign(target_position.y - position.y);

        position = target_position;
        path.pop();

        if (path.empty())
        {
            find_path(stage);
        }

        target_position.x = path.top().second*STAGE_HEIGHT + SQUARE_SIZE/2;
        target_position.y = path.top().first*STAGE_WIDTH + SQUARE_SIZE/2;

        current_speed.x = sign(target_position.x - position.x)*speed.x;
        current_speed.y = sign(target_position.y - position.y)*speed.y;

        position.x += error.x*sign(current_speed.x);
        position.y += error.y*sign(current_speed.y);

        square.first = position.y % STAGE_HEIGHT;
        square.second = position.x % STAGE_WIDTH;
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
