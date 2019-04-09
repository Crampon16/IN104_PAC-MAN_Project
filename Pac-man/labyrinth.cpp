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
                case 'X':
                    stage.matrix[i][j].obstructed = true;
                    break;
                case 'O':
                    stage.matrix[i][j].item = "gum";
                default:
                    break;
            }
        }
        ++i;
    }
    
    return stage;
}
