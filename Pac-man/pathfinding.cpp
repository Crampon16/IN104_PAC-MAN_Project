//
//  pathfinding.cpp
//  Pac-man
//
//  Created by Liam Rampon on 09/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "pathfinding.hpp"

typedef std::stack< std::pair<int, int> > SquareStack;
typedef std::pair<int, int> SquarePos;

using namespace std;

SquareStack blinky_AI(SquarePos pos, Stage& stage)
{
    SquareStack stack;
    
    return stack;
}

SquareStack pinky_AI(SquarePos pos, Stage& stage)
{
    SquareStack stack;
    
    return stack;
}
SquareStack inky_AI(SquarePos pos, Stage& stage)
{
    SquareStack stack;
    
    return stack;
}
SquareStack clyde_AI(SquarePos pos, Stage& stage)
{
    SquareStack stack;
    
    return stack;
}

//basic AI for testing : only goes one way, and stops at the edge of the stage
SquareStack pacman_AI2(SquarePos pos, Stage& stage)
{
    SquareStack stack;
    char input = stage.last_key_input;
    
    switch (input)
    {
        case 'z':
            stack.push({0,stage.entities_positions[0].second});
            break;
        case 'q':
            stack.push({stage.entities_positions[0].first,0});
            break;
        case 's':
            stack.push({STAGE_HEIGHT-1,stage.entities_positions[0].second});
            break;
        case 'd':
            stack.push({stage.entities_positions[0].first,STAGE_WIDTH-1});
            break;
        
        default:
            stack.push({stage.entities_positions[0].first,stage.entities_positions[0].second});
            break;
    }
    
    stage.last_key_input = 0;
    return stack;
}

SquareStack pacman_AI(SquarePos pos, Stage& stage)
{
    SquareStack stack;
    SquarePos current_pos = pos;
    
    char input = stage.last_key_input;
    int line_movement = 0, column_movement = 0;
    
    switch (input)
    {
        case 'z':
            line_movement = -1;
            break;
        case 'q':
            column_movement = -1;
            break;
        case 's':
            line_movement = 1;
            break;
        case 'd':
            column_movement = 1;
            break;
            
        default:
            stack.push(pos);
            return stack;
            break;
    }
    

    
    //if trying to go through a wall
    if(stage.matrix[current_pos.first + line_movement][current_pos.second + column_movement].obstructed)
    {
        stack.push(pos);
        return stack;
    }
    
    SquareStack temp_stack;
    while (not stage.matrix[current_pos.first + line_movement][current_pos.second + column_movement].obstructed)
    {
        current_pos.first += line_movement;
        current_pos.second += column_movement;
        
        if(stage.matrix[current_pos.first][current_pos.second].is_node)
            temp_stack.push(current_pos);
        
        // !!!!!!!
        //TODO: Add a control to avoir infinite loops
        // !!!!!!!
    }
    //transfer one stack into the other
    while (not temp_stack.empty())
    {
        stack.push(temp_stack.top());
        temp_stack.pop();
    }
    
    if (stack.size() == 1) //if there is a node in the way, preserve the chosen direction
        stage.last_key_input = 0;
    return stack;
}
