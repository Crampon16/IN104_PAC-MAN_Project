//
//  entity.cpp
//  Pac-man
//
//  Created by Liam Rampon on 02/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "entity.hpp"

using namespace std;

Entity::Entity(SDL_Point pos, int spd, std::stack<std::pair<int,int> > paff, std::stack<std::pair<int,int> > (*paff_finder)(int, Stage&), int ID)
{
    position = pos;

    speed = spd;

    path = paff;
    if (path.empty())
    {
        path.push({pos.y/SQUARE_SIZE, pos.x/SQUARE_SIZE});
    }
    path_finder = paff_finder;
    previous_square = path.top();

    Id = ID;
    state = NORMAL;
}

SDL_Point Entity::get_position()
{
    return position;
}

void Entity::set_position(pair<int, int> square, Stage& stage)
{
    position.x = square.second*SQUARE_SIZE + SQUARE_SIZE/2;
    position.y = square.first*SQUARE_SIZE + SQUARE_SIZE/2;
    stage.entities_positions[Id] = square;
}

void Entity::set_position(SDL_Point pos, Stage& stage)
{
    position = pos;
    stage.entities_positions[Id].first = pos.y/SQUARE_SIZE + SQUARE_SIZE/2;
    stage.entities_positions[Id].second = pos.x/SQUARE_SIZE + SQUARE_SIZE/2;
}

void Entity::set_speed(int sp)
{
    speed = sp;
}

std::pair<int, int> Entity::get_previous_square()
{
    return previous_square;
}

void Entity::set_previous_square(pair<int,int> square)
{
    previous_square = square;
}



std::vector<SDL_Rect> Entity::get_colliders()
{
    return colliders;
}

std::stack<std::pair<int, int>> Entity::get_path()
{
    return path;
}

void Entity::set_path(stack<pair<int,int>> path)
{
    this->path = path;
}


float sign(float n)
{
    if (n>0) {return 1;}
    else if (n<0) {return -1;}
    else {return 0;}
}

std::pair<int,int> Entity::get_square_position()
{
    std::pair<int, int> square;
    square.first = (position.y/SQUARE_SIZE);
    square.second = (position.x/SQUARE_SIZE);
    return square;
}

void Entity::move(int delta, Stage& stage)
{
    SDL_Point target_position;
    std::pair<int,int> target_square = path.top();

    target_position.x = target_square.second*SQUARE_SIZE + SQUARE_SIZE/2;
    target_position.y = target_square.first*SQUARE_SIZE + SQUARE_SIZE/2;

    //in this case the entity is stationnary
    if(target_position.x == position.x and target_position.y == position.y)
    {
        path.pop();
        if (path.empty())
        {
            find_path(stage);
            previous_square = target_square;
        }
        /*else if(stage.matrix[path.top().first][path.top().second].is_node)
        {
            find_path(stage);
        }*/
    }
    else
    {
        SDL_Point direction;
        direction.x = target_position.x - position.x;
        direction.y = target_position.y - position.y;

        float norm = sqrt(direction.x*direction.x + direction.y*direction.y);

        //if the movement would reach beyond the target
        if (norm <= speed*delta/1000)
        {
            position = target_position;
            stage.entities_positions[Id] = get_square_position();
            path.pop();
            if (path.empty())
            {
                find_path(stage);
                previous_square = target_square;

            }
            /*else if(stage.matrix[path.top().first][path.top().second].is_node)
            {
                find_path(stage);
            }*/
        }
        else
        {
            position.x += direction.x*speed*delta/norm/1000;
            position.y += direction.y*speed*delta/norm/1000;
            stage.entities_positions[Id] = get_square_position();
        }


    }

}

void Entity::find_path(Stage& stage)
{
    std::pair<int, int> square;
    square.first = position.y / SQUARE_SIZE;
    square.second = position.x / SQUARE_SIZE;

    path = path_finder(Id, stage);
}

void Entity::set_path_finding(std::stack<std::pair<int,int> > (*paff_finder)(int id, Stage&))
{
    path_finder = paff_finder;
}
