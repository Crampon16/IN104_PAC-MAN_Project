//
//  labyrinth.cpp
//  Pac-man
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
                    ++stage.number_of_gums;
                    break;
                case 'K': //a square containing a super-gum
                    ++stage.number_of_gums;
                    stage.matrix[i][j].item = "super_gum";
                    break;
                case 'N': //a node : contains a normal gum and entities recalculate their path here
                    stage.matrix[i][j].item = "gum";
                    ++stage.number_of_gums;
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


    //Generating a list of the sprites to superpose for the walls
    for (int line = 0; line < STAGE_HEIGHT; ++line)
        for (int column = 0; column < STAGE_WIDTH; ++column)
        {
            bitset<8> adjacent_walls = 0;
            if (line != 0)
            {
                adjacent_walls[2] = stage.matrix[line-1][column].obstructed;
                if (column != 0)
                    adjacent_walls[3] = stage.matrix[line-1][column-1].obstructed;
                if (column != STAGE_WIDTH - 1)
                    adjacent_walls[1] = stage.matrix[line-1][column+1].obstructed;
            }
            if (line != STAGE_HEIGHT - 1)
            {
                adjacent_walls[6] = stage.matrix[line+1][column].obstructed;
                if (column != 0)
                    adjacent_walls[5] = stage.matrix[line+1][column-1].obstructed;
                if (column != STAGE_WIDTH - 1)
                    adjacent_walls[7] = stage.matrix[line+1][column+1].obstructed;
            }
            if (column != 0)
                adjacent_walls[4] = stage.matrix[line][column-1].obstructed;
            if (column != STAGE_WIDTH - 1)
                adjacent_walls[0] = stage.matrix[line][column+1].obstructed;


            if ( adjacent_walls[0] + adjacent_walls[2] + adjacent_walls[4] + adjacent_walls[6] == 0 ) // round
                stage.matrix[line][column].sprites.push_back({AVATAR_SIZE, 4*AVATAR_SIZE});

            else if (  adjacent_walls[0] + adjacent_walls[2] + adjacent_walls[4] +  adjacent_walls[6] == 1 )// thin end
            {
                if (adjacent_walls[0]) // right full
                    stage.matrix[line][column].sprites.push_back({0, 4*AVATAR_SIZE});
                else if (adjacent_walls[2]) // top full
                    stage.matrix[line][column].sprites.push_back({AVATAR_SIZE, 5*AVATAR_SIZE});
                else if (adjacent_walls[4]) // left full
                    stage.matrix[line][column].sprites.push_back({2*AVATAR_SIZE, 4*AVATAR_SIZE});
                else // bottom full
                    stage.matrix[line][column].sprites.push_back({AVATAR_SIZE, 3*AVATAR_SIZE});
            }

            else // superposition time
            {
                if (adjacent_walls[2] and adjacent_walls[6]) //vertical straight wall
                {
                    if (not adjacent_walls[4])
                        stage.matrix[line][column].sprites.push_back({0, AVATAR_SIZE}); //left
                    if (not adjacent_walls[0])
                        stage.matrix[line][column].sprites.push_back({2*AVATAR_SIZE, AVATAR_SIZE}); //right
                }
                else if (adjacent_walls[0] and adjacent_walls[4]) // horizontal straight wall
                {
                    if (not adjacent_walls[2])
                        stage.matrix[line][column].sprites.push_back({AVATAR_SIZE, 0}); //top
                    if (not adjacent_walls[6])
                        stage.matrix[line][column].sprites.push_back({AVATAR_SIZE, 2*AVATAR_SIZE}); //bottom
                }

                if (adjacent_walls[0] and adjacent_walls[2]) // NE corner
                {
                    if ( not (adjacent_walls[4] or adjacent_walls[6]) ) // big
                        stage.matrix[line][column].sprites.push_back({0, 2*AVATAR_SIZE});
                    if (not adjacent_walls[1]) //small
                        stage.matrix[line][column].sprites.push_back({0, 5*AVATAR_SIZE});
                }
                if (adjacent_walls[2] and adjacent_walls[4]) // NW corner
                {
                    if ( not (adjacent_walls[0] or adjacent_walls[6]) ) // big
                        stage.matrix[line][column].sprites.push_back({2*AVATAR_SIZE, 2*AVATAR_SIZE});
                    if (not adjacent_walls[3]) //small
                        stage.matrix[line][column].sprites.push_back({2*AVATAR_SIZE, 5*AVATAR_SIZE});
                }
                if (adjacent_walls[4] and adjacent_walls[6]) // SW corner
                {
                    if ( not (adjacent_walls[0] or adjacent_walls[2]) ) // big
                        stage.matrix[line][column].sprites.push_back({2*AVATAR_SIZE, 0});
                    if (not adjacent_walls[5]) //small
                        stage.matrix[line][column].sprites.push_back({2*AVATAR_SIZE, 3*AVATAR_SIZE});
                }
                if (adjacent_walls[6] and adjacent_walls[0]) // SE corner
                {
                    if ( not (adjacent_walls[2] or adjacent_walls[4]) ) // big
                        stage.matrix[line][column].sprites.push_back({0, 0});
                    if (not adjacent_walls[7]) //small
                        stage.matrix[line][column].sprites.push_back({0, 3*AVATAR_SIZE});
                }
            }

        }

    stack<pair<int, int>> pac_path;
    pac_path.push({stage.entities_spawn_point[0]});
    stack<pair<int, int>> blink_path;
    blink_path.push({stage.entities_spawn_point[1]});
    stack<pair<int, int>> pink_path;
    pink_path.push({stage.entities_spawn_point[2]});
    stack<pair<int, int>> inky_path;
    inky_path.push({stage.entities_spawn_point[3]});
    stack<pair<int, int>> clyde_path;
    clyde_path.push({stage.entities_spawn_point[4]});

    stage.normal_pathfinder = {pacman_AI, blinky_AI, pinky_AI, inky_AI, clyde_AI};

    stage.normal_pathfinder = {pacman_AI, blinky_AI, pinky_AI};

    Entity pac( {SQUARE_SIZE/2 + stage.entities_spawn_point[0].second*SQUARE_SIZE, SQUARE_SIZE/2 + stage.entities_spawn_point[0].first*SQUARE_SIZE}, 130, pac_path , pacman_AI, 0);
    stage.entities.push_back(pac);
    stage.entities_positions.push_back(stage.entities_spawn_point[0]);

    Entity blinky( {SQUARE_SIZE/2 + stage.entities_spawn_point[1].second*SQUARE_SIZE, SQUARE_SIZE/2 + stage.entities_spawn_point[1].first*SQUARE_SIZE}, 75, blink_path , still_AI, 1);
    stage.entities.push_back(blinky);
    stage.entities_positions.push_back(stage.entities_spawn_point[1]);

    Entity pinky( {SQUARE_SIZE/2 + stage.entities_spawn_point[2].second*SQUARE_SIZE, SQUARE_SIZE/2 + stage.entities_spawn_point[2].first*SQUARE_SIZE}, 75, pink_path , still_AI, 2);
    stage.entities.push_back(pinky);
    stage.entities_positions.push_back(stage.entities_spawn_point[2]);

    Entity inky( {SQUARE_SIZE/2 + stage.entities_spawn_point[3].second*SQUARE_SIZE, SQUARE_SIZE/2 + stage.entities_spawn_point[3].first*SQUARE_SIZE}, 75, inky_path , still_AI, 3);
    stage.entities.push_back(inky);
    stage.entities_positions.push_back(stage.entities_spawn_point[3]);

    Entity clyde( {SQUARE_SIZE/2 + stage.entities_spawn_point[4].second*SQUARE_SIZE, SQUARE_SIZE/2 + stage.entities_spawn_point[4].first*SQUARE_SIZE}, 75, clyde_path , still_AI, 4);
    stage.entities.push_back(clyde);
    stage.entities_positions.push_back(stage.entities_spawn_point[4]);

    return stage;
}
