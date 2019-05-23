//
//  level_handler.cpp
//  Pac-man
//


#include "level_handler.hpp"

using namespace std;

bool classic_level(string layout, SDL_Renderer* renderer, vector<LTexture*> const &textures, LBitmapFont& font, vector<Mix_Chunk*> const &sounds, vector<Mix_Music*> const& tracks, bool &quit)
{

    Stage stage = init_stage(layout);

    Uint32 start_time = SDL_GetTicks();


    SDL_Event e;
    bool victory = false, level = true;

    FPSCapper cap(60);

    stage.killer_mode_start = 0;
    stage.level_start = SDL_GetTicks();

    sound_animation(renderer, textures, font, stage, sounds[0], new_game_music_time, quit);
    Mix_PlayMusic(tracks[0], -1);

    while( !quit and level)
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
        handle_collisions(stage, sounds, renderer, textures, font, quit, start_time);
        handle_AIs(stage);
        display(renderer, stage, font, textures);

        if(stage.lives == -1)
            level = false;
        else if(stage.number_of_gums <= 0)
        {
            level = false;
            victory = true;

            Mix_HaltMusic();
        }

        cap.cap();
    }

    return victory;
}

void display_splash(SDL_Renderer* renderer, LTexture* splash , Uint32 time, bool& quit)
{
    SDL_Event e;
    FPSCapper cap(60);

    Uint32 start = SDL_GetTicks();

    while( !quit and SDL_GetTicks() - start < time)
    {
        cap.start();
        while (SDL_PollEvent( &e ) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        SDL_Point splash_position = { (SCREEN_WIDTH - splash->getWidth())/2, (SCREEN_HEIGHT - splash->getHeight())/2};
        splash->render(splash_position);

        SDL_RenderPresent(renderer);

        cap.cap();
    }
}

void handle_collisions(Stage& stage, vector<Mix_Chunk*> const &sounds, SDL_Renderer* renderer, vector<LTexture*> const &textures, LBitmapFont& font, bool &quit, Uint32 duration)
{
    // pac/gum collision
    pair<int, int> pac_pos = stage.entities_positions[0];
    if(stage.matrix[pac_pos.first][pac_pos.second].item == "gum")
    {
        stage.score += 10;
        --stage.number_of_gums;
        stage.matrix[pac_pos.first][pac_pos.second].item = "";

        //play sound
        Mix_PlayChannel( -1, sounds[1], 0 );
    }

    // pac/super-gum collision
    if(stage.matrix[pac_pos.first][pac_pos.second].item == "super_gum")
    {
        stage.score += 100;
        --stage.number_of_gums;

        //enter killer mode
        stage.entities[0].state = KILLER;
        stage.killer_mode_start = SDL_GetTicks();

        for (int i = 1; i <= 4; ++i)
        {
            //if the ghost is still, no effect
            if (stage.entities[i].get_path_finding() == still_AI)
                continue;

            //put ghost in afraid state, make him flee pac, and reverse its current direction
            stage.entities[i].state = AFRAID;

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
                    //Death animation
                    sound_animation(renderer, textures, font, stage, sounds[4], 1700, quit);

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
                    Mix_PlayChannel( -1, sounds[2], 0 );
                    break;
                }

                    //nothing to do in this case yet
                case DEAD:
                default:
                    break;
            }



            break;
        }

        if (77000 < SDL_GetTicks()-duration and SDL_GetTicks()-duration < 78000)
            stage.entities[i].set_speed(BOOSTED_SPEED);

    }

}

void handle_AIs(Stage& stage)
{
    //only ghosts need an AI change
    for (int i = 1; i <= 4; ++i)
    {
        //check if the ghost should stay still
        if (SDL_GetTicks() - stage.level_start < (i-1)*2500 + new_game_music_time)
            continue;

        if (stage.entities[i].get_path_finding() == still_AI)
        {
            stage.entities[i].set_path_finding(stage.normal_pathfinder[i-1]);
        }

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

void sound_animation(SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font, Stage& stage, Mix_Chunk* const &sound, Uint32 duration, bool &quit)
{
    SDL_Event e;
    Uint32 time_beginning = SDL_GetTicks();

    FPSCapper cap(60);

    if( Mix_PlayingMusic() == 1 )
        Mix_PauseMusic();

    Mix_PlayChannel( -1, sound, 0 );

    while ( !quit and SDL_GetTicks()-time_beginning < duration)
    {
        cap.start();
        while (SDL_PollEvent( &e ) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        display(renderer, stage, font, textures);

        cap.cap();
    }
    Mix_ResumeMusic();
}
