//
//  labyrinth.cpp
//  Pac-man
//
//  Created by Liam Rampon on 03/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "labyrinth.hpp"

using namespace std;

//string splitting function
void split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

Stage init_stage(string path)
{
    //initializes an empty Stage
    Stage stage;
    Square empty_square;
    for (int i = 0; i < STAGE_HEIGHT; ++i)
    {
        vector<Square> empty_line;
        stage.matrix.push_back(empty_line);
        for (int j = 0; j < STAGE_WIDTH; ++j)
        {
            stage.matrix[i].push_back(empty_square);
        }
    }
    stage.entities_spawn_point.resize(5);
    stage.entities_spawn_direction.resize(5);

    //open stream to read layout file
    ifstream stream(path);
    if(not stream)
    {
        cout << "ERROR: Unable to open the stage layout file." << endl;
        return stage;
    }
    string buffer;
    vector<string> params;
    
    int i = 0;
    while ( getline(stream, buffer) )
    {
        for(int j = 0; j < STAGE_WIDTH; ++j)
        {
            switch (buffer[j])
            {

                case 'X': //a wall
                    stage.matrix[i][j].obstructed = true;
                    break;
                case 'O': //a square containing a gum
                    stage.matrix[i][j].item = "gum";
                    break;
                case 'K': //a square containing a super-gum
                    stage.matrix[i][j].item = "super_gum";
                    break;
                case 'N': //a node : contains a normal gum and entities recalculate their path here
                    stage.matrix[i][j].item = "gum";
                    stage.matrix[i][j].is_node = true;
                    break;
                case 'M': //a node without gum
                    stage.matrix[i][j].is_node = true;
                    break;
                case 'U': //a square where you can only go up, and only enter by the sides or the lower half
                    stage.matrix[i][j].go_up = true;
                    break;
                
                    //every spawn point (and spawn direction) is a node
                case 'S': //pac-man's spawn AND spawn direction
                    stage.entities_spawn_point[0] = {i,j};
                    stage.entities_spawn_direction[0] = {i,j};
                    stage.matrix[i][j].is_node = true;
                    break;
                case 'B': //spawn of blinky
                    stage.entities_spawn_point[1] = {i,j};
                    break;
                case 'b': //spawn direction of blinky
                    stage.entities_spawn_direction[1] = {i,j};
                    stage.matrix[i][j].is_node = true;
                    break;
                case 'P': //spawn of pinky
                    stage.entities_spawn_point[2] = {i,j};
                    break;
                case 'p': //spawn direction of pinky
                    stage.entities_spawn_direction[2] = {i,j};
                    stage.matrix[i][j].is_node = true;
                    break;
                case 'I': //spawn of inky
                    stage.entities_spawn_point[3] = {i,j};
                    break;
                case 'i': //spawn direction of inky
                    stage.entities_spawn_direction[3] = {i,j};
                    stage.matrix[i][j].is_node = true;
                    break;
                case 'C': //spawn of clide
                    stage.entities_spawn_point[4] = {i,j};
                    break;
                case 'c': //spawn direction of clide
                    stage.entities_spawn_direction[4] = {i,j};
                    stage.matrix[i][j].is_node = true;
                    break;
                    
                    
                default:
                    break;
            }
        }
        ++i;
    }
    
    stack<pair<int, int>> pac_path;
    pac_path.push({stage.entities_spawn_point[0]});
    stack<pair<int, int>> blink_path;
    blink_path.push({stage.entities_spawn_point[1]});
    stack<pair<int, int>> pink_path;
    pink_path.push({stage.entities_spawn_point[2]});

    
    Entity pac( {SQUARE_SIZE/2 + stage.entities_spawn_point[0].second*SQUARE_SIZE, SQUARE_SIZE/2 + stage.entities_spawn_point[0].first*SQUARE_SIZE}, 150, pac_path , pacman_AI, 0);
    stage.entities.push_back(pac);
    stage.entities_positions.push_back(stage.entities_spawn_point[0]);
    
    Entity blinky( {SQUARE_SIZE/2 + stage.entities_spawn_point[1].second*SQUARE_SIZE, SQUARE_SIZE/2 + stage.entities_spawn_point[1].first*SQUARE_SIZE}, 75, blink_path , blinky_AI, 1);
    stage.entities.push_back(blinky);
    stage.entities_positions.push_back(stage.entities_spawn_point[1]);
    
    Entity pinky( {SQUARE_SIZE/2 + stage.entities_spawn_point[2].second*SQUARE_SIZE, SQUARE_SIZE/2 + stage.entities_spawn_point[2].first*SQUARE_SIZE}, 75, pink_path , pinky_AI, 2);
    stage.entities.push_back(pinky);
    stage.entities_positions.push_back(stage.entities_spawn_point[2]);
    
    return stage;
}


