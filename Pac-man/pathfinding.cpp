//
//  pathfinding.cpp
//  Pac-man
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


//pathfinding functions

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
                
                //if the node  is unexplored, it has been reached by the shortest path
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
    if (stage.matrix[start.first][start.second].go_up)
    {
        SquareStack path;
        path.push({start.first - 1, start.second});
        return path;
    }
    
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
        //valid square to go to: no wall and not going on a up-square from the top
        if (candidate != previous and not stage.matrix[candidate.first][candidate.second].obstructed
            and not (candidate.first - start.first == 1 and stage.matrix[candidate.first][candidate.second].go_up))
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
        if (candidate != previous and not stage.matrix[candidate.first][candidate.second].obstructed
            and not (candidate.first - start.first == 1 and stage.matrix[candidate.first][candidate.second].go_up))
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

SquareStack farthest_square(Stage& stage, SquarePos start, SquarePos goal, SquarePos previous)
{
    if (stage.matrix[start.first][start.second].go_up)
    {
        SquareStack path;
        path.push({start.first - 1, start.second});
        return path;
    }
    
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
        if (candidate != previous and not stage.matrix[candidate.first][candidate.second].obstructed
            and not (candidate.first - start.first == 1 and stage.matrix[candidate.first][candidate.second].go_up))
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
        if (candidate != previous and not stage.matrix[candidate.first][candidate.second].obstructed
            and not (candidate.first - start.first == 1 and stage.matrix[candidate.first][candidate.second].go_up))
        {
            //compare distance to goal
            if (square_distance(goal, best) < square_distance(goal, candidate))
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




//normal AIs of the ghosts

SquareStack blinky_AI(int id, Stage& stage)
{
    return nearest_square(stage, stage.entities_positions[id], stage.entities_positions[pac_man_id], stage.entities[id].get_previous_square());
}

SquareStack pinky_AI(int id, Stage& stage)
{
    SquarePos base = stage.entities_positions[pac_man_id], prev = stage.entities[pac_man_id].get_previous_square();
    SquarePos offset = {4*(base.first - prev.first), 4*(base.second - prev.second)};
    SquarePos goal = {base.first + offset.first, base.second + offset.second};
    
    return nearest_square(stage, stage.entities_positions[id], goal, stage.entities[id].get_previous_square());
}

SquareStack inky_AI(int id, Stage& stage)
{
    //not finished
    
    return nearest_square(stage, stage.entities_positions[id], stage.entities_positions[pac_man_id], stage.entities[id].get_previous_square());
}

SquareStack clyde_AI(int id, Stage& stage)
{
    int distance = square_distance(stage.entities_positions[id], stage.entities_positions[pac_man_id]);
    
    if (distance <= clyde_chicken_distance)
        return farthest_square(stage, stage.entities_positions[id], stage.entities_positions[pac_man_id], stage.entities[id].get_previous_square());
    
    return nearest_square(stage, stage.entities_positions[id], stage.entities_positions[pac_man_id], stage.entities[id].get_previous_square());
}



//AIs for frightened or dead ghosts

SquareStack still_AI(int id, Stage& stage)
{
    SquareStack stack;
    stack.push(stage.entities_positions[id]);
    return stack;
}

SquareStack scatter_AI(int id, Stage& stage)
{
    //scatter points are the edges of the stage
    SquareStack path;
    switch (id)
    {
        case 1:
            return nearest_square(stage, stage.entities_positions[id], {0,0}, stage.entities[id].get_previous_square());
            break;
        case 2:
            return nearest_square(stage, stage.entities_positions[id], {STAGE_HEIGHT,0}, stage.entities[id].get_previous_square());
            break;
        case 3:
            return nearest_square(stage, stage.entities_positions[id], {0,STAGE_WIDTH}, stage.entities[id].get_previous_square());
            break;
        case 4:
            return nearest_square(stage, stage.entities_positions[id], {STAGE_HEIGHT,STAGE_WIDTH}, stage.entities[id].get_previous_square());
            break;
        default:
            path.push(stage.entities_positions[id]);
            return path;
            break;
    }
}

SquareStack escape_AI(int id, Stage& stage)
{
    return farthest_square(stage, stage.entities_positions[id], stage.entities_positions[0], stage.entities[id].get_previous_square());
}

SquareStack death_AI(int id, Stage& stage)
{
    SquareStack stack;
    stack.push(stage.entities_spawn_direction[id]);
    return stack;
}



//AI of the player avatar

SquareStack pacman_AI(int id, Stage& stage)
{
    SquareStack stack;
    SquarePos current_pos = stage.entities_positions[id];
    
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
