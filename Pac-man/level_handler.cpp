
//
//  level_handler.cpp
//  Pac-man
//
//  Created by Liam Rampon on 25/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "level_handler.hpp"

using namespace std;

void classic_level(string layout, SDL_Renderer* renderer, vector<LTexture*> const &textures, LBitmapFont& font)
{

    Stage stage = init_stage(layout);


    SDL_Event e;
    bool quit = false;

    FPSCapper cap(60);

    stage.killer_mode_start = 0;

    while( !quit )
    {
        cap.start();
        while (SDL_PollEvent( &e ) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
            get_keyboard_input(stage, e);
        }
        for (int i = 0; i < stage.entities.size(); ++i)
        {
            // 1/60s = 17ms
            stage.entities[i].move(17, stage);
        }
        handle_collisions(stage);
        handle_AIs(stage);
        display(renderer, stage, font, textures);

        if(stage.lives == -1)
            quit = true;

        //cout << (stage.entities[1].state == AFRAID) << (stage.entities[1].state == DEAD) << endl;
        //cout << (stage.entities[2].state == AFRAID) << (stage.entities[2].state == DEAD) << endl;

        cap.cap();
    }
}

void handle_collisions(Stage& stage)
{
    // pac/gum collision
    pair<int, int> pac_pos = stage.entities_positions[0];
    if(stage.matrix[pac_pos.first][pac_pos.second].item == "gum")
    {
        stage.score += 10;
        stage.matrix[pac_pos.first][pac_pos.second].item = "";
    }

    // pac/super-gum collision
    if(stage.matrix[pac_pos.first][pac_pos.second].item == "super_gum")
    {
        stage.score += 100;

        //enter killer mode
        stage.entities[0].state = KILLER;
        stage.killer_mode_start = SDL_GetTicks();

        for (int i = 1; i <= 4; ++i)
        {
            //put ghost in afraid state, make him flee pac, and reverse its current direction
            stage.entities[i].state = AFRAID;

            pair<int, int> tmp = stage.entities[i].get_previous_square();
            stage.entities[i].set_previous_square(stage.entities[i].get_path().top());

            stage.entities[i].set_path_finding(escape_AI);
            stage.entities[i].find_path(stage);
        }

        stage.matrix[pac_pos.first][pac_pos.second].item = "";
    }

    // pac/ghost collision
    for (int i = 1; i <= 4; ++i)
    {
        if (stage.entities_positions[0] == stage.entities_positions[i])
        {
            switch (stage.entities[i].state)
            {
                //in this case, pac man dies
                case NORMAL:
                {
                    stack<pair<int, int>> pac_path, blink_path, pink_path, inky_path, clyde_path;
                    pac_path.push(stage.entities_spawn_point[0]);
                    blink_path.push({stage.entities_spawn_point[1]});
                    pink_path.push({stage.entities_spawn_point[2]});
                    inky_path.push({stage.entities_spawn_point[3]});
                    clyde_path.push({stage.entities_spawn_point[4]});


                    stage.entities[0].set_position(stage.entities_spawn_point[0], stage);
                    stage.entities[0].set_path(pac_path);
                    stage.entities[1].set_position(stage.entities_spawn_point[1], stage);
                    stage.entities[1].set_path(blink_path);
                    stage.entities[2].set_position(stage.entities_spawn_point[2], stage);
                    stage.entities[2].set_path(pink_path);
                    stage.entities[3].set_position(stage.entities_spawn_point[3], stage);
                    stage.entities[3].set_path(inky_path);
                    stage.entities[4].set_position(stage.entities_spawn_point[4], stage);
                    stage.entities[4].set_path(clyde_path);

                    stage.last_key_input = ' ';
                    --stage.lives;
                    break;
                }

                //in this case, the ghost dies
                case AFRAID:
                {
                    //put the ghost in the dead state and increase score
                    stage.entities[i].state = DEAD;
                    stage.entities[i].set_path(bfs(stage, stage.entities_positions[i], stage.entities_spawn_point[i]));
                    stage.entities[i].set_speed(200);

                    stage.score += 200;
                    break;
                }

                //nothing to do in this case yet
                case DEAD:
                default:
                    break;
            }



            break;
        }
    }
}

void handle_AIs(Stage& stage)
{
    //only ghosts need an AI change
    for (int i = 1; i <= 4; ++i)
    {
        //if a dead ghost reached its spawn
        if (stage.entities[i].state == DEAD and stage.entities_positions[i] == stage.entities_spawn_point[i])
        {
            stage.entities[i].state = NORMAL;
            stage.entities[i].set_speed(75);
        }
        else
        {
            //killer mode only lasts for 5s
            if(stage.entities[i].state == AFRAID and SDL_GetTicks() - stage.killer_mode_start > 5000)
            {
                stage.entities[i].state = NORMAL;
                stage.entities[i].set_path_finding(stage.normal_pathfinder[i]);
            }

            //every 5s, ghosts enter chase mode for 15s, before scattering again for 5s and so on
            if (SDL_GetTicks() % 20000 < 5000)
                stage.entities[i].set_path_finding(scatter_AI);
            else
                stage.entities[i].set_path_finding(stage.normal_pathfinder[i]);

        }
    }
}
