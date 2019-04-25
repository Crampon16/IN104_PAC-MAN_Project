//
//  level_handler.cpp
//  Pac-man
//
//  Created by Liam Rampon on 25/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "level_handler.hpp"

using namespace std;

void classic_level(string layout, SDL_Renderer* renderer, vector<LTexture*> textures, LBitmapFont& font)
{
    Stage stage = init_stage(layout);
    
    SDL_Event e;
    bool quit = false;
    
    
    FPSCapper cap(60);
    
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
        display(renderer, stage, font);
        
        if(stage.lives == -1)
            quit = true;
        
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
        for (int i = 1; i <= stage.entities.size(); ++i)
        {
            stage.entities[i].state = AFRAID;
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
                    stack<pair<int, int>> pac_path, blink_path, pink_path;
                    pac_path.push(stage.entities_spawn_point[0]);
                    blink_path.push({stage.entities_spawn_point[1]});
                    pink_path.push({stage.entities_spawn_point[2]});
                    
                    stage.entities[0].set_position(stage.entities_spawn_point[0], stage);
                    stage.entities[0].set_path(pac_path);
                    stage.entities[1].set_position(stage.entities_spawn_point[1], stage);
                    stage.entities[1].set_path(blink_path);
                    stage.entities[2].set_position(stage.entities_spawn_point[2], stage);
                    stage.entities[2].set_path(pink_path);
                    
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
    for (int i = 0; i <= 4; ++i)
    {
        if (stage.entities[i].state == DEAD and stage.entities_positions[i] == stage.entities_spawn_point[i])
        {
            stage.entities[i].state = NORMAL;
            stage.entities[i].set_speed(75);
        }
    }
}

