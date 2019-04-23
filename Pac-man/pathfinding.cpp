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
        if(stage.matrix[current.first][current.second].is_node)
            path.push(current);
        current = nodes[current.first][current.second].previous;
    }
    return path;
}

SquareStack blinky_AI(SquarePos pos, Stage& stage)
{
    return bfs(stage, pos, stage.entities_positions[0]);
}

SquareStack pinky_AI(SquarePos pos, Stage& stage)
{
    SquareStack pac_path = stage.entities[0].get_path();
    
    return bfs(stage, pos, pac_path.top());
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
        SquareStack ori_path = stage.entities[0].get_path();
        if (not ori_path.empty())
            return ori_path;
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
