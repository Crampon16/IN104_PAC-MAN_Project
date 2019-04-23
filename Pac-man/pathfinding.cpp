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


//Node structure for bfs/dfs
struct Node
{
    SquarePos previous = {-1, -1};
};

SquareStack bfs(Stage& stage, SquarePos start, SquarePos goal)
{
    SquarePos unexplored = {-1, -1};
    
    //The queue will stock squares to process, the array the results of the processing
    queue<SquarePos> unexplored_nodes;
    Node nodes[STAGE_HEIGHT][STAGE_WIDTH];
    
    unexplored_nodes.push(start);
    
    while(not unexplored_nodes.empty())
    {
        SquarePos current = unexplored_nodes.front();
        unexplored_nodes.pop();
        
        if (current.first > 0)
        {
            SquarePos candidate = {current.first - 1, current.second};
            if(not stage.matrix[candidate.first][candidate.second].obstructed)
            {
                //if the goal is reached, it is mandatorily by the shortest path
                //the goal has the way to it updated and the loop is broken
                if(candidate == goal)
                {
                    nodes[candidate.first][candidate.second].previous = current;
                    break;
                }
                
                //if the node is unexplored, it has been reached by the shortest path
                if (nodes[candidate.first][candidate.second].previous == unexplored)
                {
                    nodes[candidate.first][candidate.second].previous = current;
                    unexplored_nodes.push(candidate);
                }
            }
        }
        if (current.second > 0)
        {
            SquarePos candidate = {current.first, current.second - 1};
            if(not stage.matrix[candidate.first][candidate.second].obstructed)
            {
                //if the goal is reached, it is mandatorily by the shortest path
                //the goal has the way to it updated and the loop is broken
                if(candidate == goal)
                {
                    nodes[candidate.first][candidate.second].previous = current;
                    break;
                }
                
                //if the node is unexplored, it has been reached by the shortest path
                if (nodes[candidate.first][candidate.second].previous == unexplored)
                {
                    nodes[candidate.first][candidate.second].previous = current;
                    unexplored_nodes.push(candidate);
                }
            }
        }
        if (current.first < STAGE_HEIGHT - 1)
        {
            SquarePos candidate = {current.first + 1, current.second};
            if(not stage.matrix[candidate.first][candidate.second].obstructed)
            {
                //if the goal is reached, it is mandatorily by the shortest path
                //the goal has the way to it updated and the loop is broken
                if(candidate == goal)
                {
                    nodes[candidate.first][candidate.second].previous = current;
                    break;
                }
                
                //if the node is unexplored, it has been reached by the shortest path
                if (nodes[candidate.first][candidate.second].previous == unexplored)
                {
                    nodes[candidate.first][candidate.second].previous = current;
                    unexplored_nodes.push(candidate);
                }
            }
        }
        if (current.second < STAGE_WIDTH - 1)
        {
            SquarePos candidate = {current.first, current.second + 1};
            if(not stage.matrix[candidate.first][candidate.second].obstructed)
            {
                //if the goal is reached, it is mandatorily by the shortest path
                //the goal has the way to it updated and the loop is broken
                if(candidate == goal)
                {
                    nodes[candidate.first][candidate.second].previous = current;
                    break;
                }
                
                //if the node is unexplored, it has been reached by the shortest path
                if (nodes[candidate.first][candidate.second].previous == unexplored)
                {
                    nodes[candidate.first][candidate.second].previous = current;
                    unexplored_nodes.push(candidate);
                }
            }
        }
    }
    
    //There was no path to the goal : stay stationnary
    //!!!!!
    //Remember to throw an error here
    //!!!!!
    if (nodes[goal.first][goal.second].previous == unexplored)
    {
        SquareStack path;
        path.push(start);
        return path;
    }
    
    
    SquareStack path;
    path.push(goal);
    SquarePos current = nodes[goal.first][goal.second].previous;
    while(current != start)
    {
        //if(stage.matrix[current.first][current.second].is_node)
            path.push(current);
        current = nodes[current.first][current.second].previous;
    }
    return path;
}

int square_distance(SquarePos square1, SquarePos square2)
{
    return (square1.first - square2.first)*(square1.first - square2.first) + (square1.second - square2.second)*(square1.second - square2.second);
}

SquareStack nearest_square(Stage& stage, SquarePos start, SquarePos goal, SquarePos previous)
{
    SquareStack candidates;
    candidates.push({start.first - 1, start.second});
    candidates.push({start.first + 1, start.second});
    candidates.push({start.first, start.second - 1});
    candidates.push({start.first, start.second + 1});
    
    //find first element of stack that can be gone through
    SquarePos best;
    bool found_first = false;
    while (not found_first)
    {
        SquarePos candidate = candidates.top();
        //valid square to go to
        if (candidate != previous and not stage.matrix[candidate.first][candidate.second].obstructed)
        {
            found_first = true;
            best = candidate;
        }
        candidates.pop();
    }
    
    //compare valid elements to find the best, euclidean distance wise
    while (not candidates.empty())
    {
        SquarePos candidate = candidates.top();
        //valid square to go to
        if (candidate != previous and not stage.matrix[candidate.first][candidate.second].obstructed)
        {
            //compare distance to goal
            if (square_distance(goal, best) > square_distance(goal, candidate))
            {
                best = candidate;
            }
        }
        candidates.pop();
    }
    
    SquareStack path;
    path.push(best);
    return path;
}

//dummied out version using bfs
/*
SquareStack blinky_AI2(int id, SquarePos pos, Stage& stage)
{
    return bfs(stage, pos, stage.entities_positions[0]);
}*/

SquareStack blinky_AI(int id, SquarePos pos, Stage& stage)
{
    
    return nearest_square(stage, pos, stage.entities_positions[0], stage.entities[id].get_previous_square());
}

//dummied out version using bfs
/*
SquareStack pinky_AI2(int id, SquarePos pos, Stage& stage)
{
    SquareStack pac_path = stage.entities[0].get_path();
    
    return bfs(stage, pos, pac_path.top());
}*/

SquareStack pinky_AI(int id, SquarePos pos, Stage& stage)
{
    SquarePos base = stage.entities_positions[0], prev = stage.entities[0].get_previous_square();
    SquarePos offset = {4*(base.first - prev.first), 4*(base.second - prev.second)};
    SquarePos goal = {base.first + offset.first, base.second + offset.second};
    
    return nearest_square(stage, pos, goal, stage.entities[id].get_previous_square());
}

SquareStack inky_AI(int id, SquarePos pos, Stage& stage)
{
    SquareStack stack;
    
    return stack;
}

SquareStack clyde_AI(int id, SquarePos pos, Stage& stage)
{
    SquareStack stack;
    
    return stack;
}

//basic AI for testing : only goes one way, and stops at the edge of the stage
SquareStack pacman_AI2(int id, SquarePos pos, Stage& stage)
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

SquareStack pacman_AI3(int id, SquarePos pos, Stage& stage)
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
        SquareStack ori_path = stage.entities[0].get_path();
        if (not ori_path.empty())
            return ori_path;
        stack.push(pos);
        return stack;
    }
    
    //stack.push({current_pos.first + line_movement, current_pos.second + column_movement});
    //return stack;
    
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

SquareStack pacman_AI(int id, SquarePos pos, Stage& stage)
{
    SquareStack stack;
    SquarePos current_pos = pos;
    
    char input = stage.last_key_input;
    int line_movement = 0, column_movement = 0;
    
    //calculate the position if the key press is followed
    bool no_press = false;
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
            no_press = true;
            break;
    }
    
    
    //if not going through a wall, return this square
    if(not stage.matrix[current_pos.first + line_movement][current_pos.second + column_movement].obstructed)
    {
        stack.push({current_pos.first + line_movement, current_pos.second + column_movement});
        return stack;
    }
    
    //if we are here, try to go in the same direction as before
    line_movement = current_pos.first - stage.entities[id].get_previous_square().first;
    column_movement = current_pos.second - stage.entities[id].get_previous_square().second;
    if(not stage.matrix[current_pos.first + line_movement][current_pos.second + column_movement].obstructed)
    {
        stack.push({current_pos.first + line_movement, current_pos.second + column_movement});
        return stack;
    }
    
    //if all else failed, stay still
    stack.push({current_pos.first, current_pos.second});
    return stack;
}
