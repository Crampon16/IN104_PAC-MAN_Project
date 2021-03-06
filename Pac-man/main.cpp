
//
//  main.cpp
//  Pac-man
//


#include <iostream>
#include <unordered_map>
#include <vector>
#include <time.h>

#include "SDL.h"
#include "SDL_mixer.h"

#include "constants.hpp"
#include "texture_wrapper.hpp"
#include "time_wrapper.hpp"
#include "entity.hpp"
#include "graphic_display.hpp"
#include "pathfinding.hpp"
#include "controller.hpp"

#include "level_handler.hpp"
#include "boss_handler.hpp"

#include "entity_rebirth.hpp"
#include "stage_reborn.hpp"

using namespace std;

bool init(SDL_Window** window, SDL_Renderer** renderer)
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        *window = SDL_CreateWindow( "Pac Man", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            *renderer = SDL_CreateRenderer( *window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( renderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( *renderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }

                //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia(vector<LTexture*>& textures, vector<string> file_path, SDL_Renderer* renderer)
{
    //Loading success flag
    bool success = true;

    //Load press texture
    for(int i = 0; i < file_path.size(); i++)
    {
        //create new texture and add it to textures
        LTexture* temp = new LTexture(renderer);
        textures.push_back(temp);

        //load the texture at the given path
        if( !textures[i]->loadFromFile( file_path[i] ) )
        {
            printf( "Failed to load texture!\n" );
            success = false;
        }
    }


    return success;
}

bool loadFont(LTexture* bitmap_texture, LBitmapFont* font, string path)
{
    //Loading success flag
    bool success = true;

    //Load font texture
    if( !bitmap_texture->loadFromFile( path ) )
    {
        printf( "Failed to load corner texture!\n" );
        success = false;
    }
    else
    {
        //Build font from texture
        font->buildFont( bitmap_texture );
    }

    return success;
}

bool loadSounds(vector<Mix_Chunk*>& sounds, vector<Mix_Music*>& tracks, vector<string> sound_path, vector<string> track_path)
{
    bool success = true;

    //Load sound effects
    for (int i = 0; i < sound_path.size(); ++i)
    {
        sounds.push_back( Mix_LoadWAV(sound_path[i].c_str()) );
        if( sounds[sounds.size() - 1] == NULL )
        {
            printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
    }


    //Load music
    for (int i = 0; i < track_path.size(); ++i)
    {
        tracks.push_back ( Mix_LoadMUS( track_path[i].c_str() ) );
        if( tracks[tracks.size() - 1] == NULL )
        {
            printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
            success = false;
        }
    }


    return success;
}

void close(SDL_Window* window, SDL_Renderer* renderer, vector<LTexture*>& textures, vector<Mix_Chunk*>& sounds, vector<Mix_Music*>& tracks)
{
    //Free loaded images
    for(int i = 0; i < textures.size(); i++)
    {
        textures[i]->free();
    }

    //Free the sound effects
    for (int i = 0; i < sounds.size(); ++i)
    {
        Mix_FreeChunk( sounds[i] );
        sounds[i] = NULL;
    }

    //Free the music
    for (int i = 0; i < tracks.size(); ++i)
    {
        Mix_FreeMusic( tracks[i] );
        tracks[i] = NULL;
    }


    //Destroy window
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    window = NULL;
    renderer = NULL;

    //Quit SDL subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argv, char** args)
{
    //This line initializes the RNG with the current date, which allows for randomness to occur
    //between different games
    srand( (unsigned int) time(0) );

    SDL_Window* window;
    SDL_Renderer* renderer;

    if( init(&window, &renderer) )
    {
        cout << "Initialization complete." << endl;

        //game textures and sounds
        vector<LTexture*> normal_stage_textures, boss_stage_textures, splash_screens;
        vector<Mix_Chunk*> sounds;
        vector<Mix_Music*> tracks;
        vector<string> stage_path, boss_path, splash_path, sound_path, track_path;

        LBitmapFont font;
        LTexture font_texture(renderer);
        string font_path = "lazyfont_blanc.png";

        stage_path.push_back("sprites/pacman_anim.png");

        stage_path.push_back("sprites/blinky_anim.png");
        stage_path.push_back("sprites/pinky_anim.png");
        stage_path.push_back("sprites/inky_anim.png");
        stage_path.push_back("sprites/clyde_anim.png");

        stage_path.push_back("sprites/frightened_anim.png");
        stage_path.push_back("sprites/blinking_anim.png");
        stage_path.push_back("sprites/dead_sprite.png");

        boss_path = stage_path;

        stage_path.push_back("sprites/wall_for_superposition.png");

        boss_path.push_back("sprites/boss_anim.png");
        boss_path.push_back("sprites/frightened_boss_anim.png");
        boss_path.push_back("sprites/blinking_boss_anim.png");

        splash_path.push_back("sprites/splash_win.png");
        splash_path.push_back("sprites/splash_boss.png");
        splash_path.push_back("sprites/splash_lost.png");

        //Sounds and music
        sound_path.push_back("Sounds/new_game.wav");
        sound_path.push_back("Sounds/gum.wav");
        sound_path.push_back("Sounds/ghost_death.wav");
        sound_path.push_back("Sounds/getting_hit.wav");
        sound_path.push_back("Sounds/pacman_death.wav");

        track_path.push_back("Sounds/classic_music.wav");
        track_path.push_back("Sounds/Boss_music.wav");
        track_path.push_back("Sounds/Tetriste.wav");
        track_path.push_back("Sounds/victory.wav");
        track_path.push_back("Sounds/victory, or not.wav");
        //Mix_PlayChannel( -1, gHigh, 0 );

        cout << "before loading" << endl;

        if ( loadMedia(normal_stage_textures, stage_path, renderer)
            and loadMedia(boss_stage_textures, boss_path, renderer)
            and loadMedia(splash_screens, splash_path, renderer)
            and loadFont(&font_texture, &font, font_path)
            and loadSounds(sounds, tracks, sound_path, track_path))
        {

            string layout = "layout2.txt";

            bool quit = false;
            //if(true /*classic_level(layout, renderer, normal_stage_textures, font, sounds, tracks, quit)*/)
            if(classic_level(layout, renderer, normal_stage_textures, font, sounds, tracks, quit))
            {
                Mix_PlayMusic(tracks[4], -1);
                display_splash(renderer,splash_screens[0], 4000, quit);
                
                //level 2
                layout = "layout3.txt";
                
                if(classic_level(layout, renderer, normal_stage_textures, font, sounds, tracks, quit))
                {
                    bool victory;
                    Mix_PlayMusic(tracks[4], -1);
                    display_splash(renderer,splash_screens[0], 4000, quit);
                    //boss level
                    Mix_PlayMusic(tracks[1], -1);
                    display_splash(renderer,splash_screens[1], 3000, quit);
                    boss_fight(renderer, boss_stage_textures, font, sounds, tracks, victory, quit );
                    if (victory)
                    {
                        Mix_PlayMusic(tracks[3], -1);
                        display_splash(renderer, splash_screens[0], 100000, quit);
                    }
                    else
                    {
                        Mix_PlayMusic(tracks[2], -1);
                        display_splash(renderer, splash_screens[2], 21000, quit);
                    }
                }
                else
                {
                    Mix_PlayMusic(tracks[2], -1);
                    display_splash(renderer, splash_screens[2], 21000, quit);
                }
            }
            else
            {
                Mix_PlayMusic(tracks[2], -1);
                display_splash(renderer, splash_screens[2], 21000, quit);
            }

            normal_stage_textures.insert(normal_stage_textures.end(), boss_stage_textures.begin(), boss_stage_textures.end());
            close(window, renderer, normal_stage_textures, sounds, tracks);

        }
    }
}
