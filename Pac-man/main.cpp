//
//  main.cpp
//  Pac-man
//
//  Created by Liam Rampon on 02/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include <iostream>
#include <unordered_map>
#include <vector>

#include "SDL2/SDL.h"

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
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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

void close(SDL_Window* window, SDL_Renderer* renderer, vector<LTexture*> textures)
{
    //Free loaded images
    for(int i = 0; i < textures.size(); i++)
    {
        textures[i]->free();
    }
    
    //Destroy window
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    window = NULL;
    renderer = NULL;
    
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argv, char** args)
{
    //This line initializes the RNG with the current date, which allows for randomness to occur
    //between different games
    srand( (uint) time(0) );

    SDL_Window* window;
    SDL_Renderer* renderer;
    
    if( init(&window, &renderer) )
    {
        cout << "Initialization complete." << endl;
        
        //game textures
        vector<LTexture*> textures;
        vector<string> file_path;
        
        file_path.push_back("sprites/pacman_anim.png");
        
        file_path.push_back("sprites/blinky_anim.png");
        file_path.push_back("sprites/pinky_anim.png");
        file_path.push_back("sprites/inky_anim.png");
        file_path.push_back("sprites/clyde_anim.png");

        file_path.push_back("sprites/frightened_anim.png");
        file_path.push_back("sprites/blinking_anim.png");
        file_path.push_back("sprites/dead_sprite.png");
        
        file_path.push_back("sprites/boss_anim.png");
        file_path.push_back("sprites/frightened_boss_anim.png");
        file_path.push_back("sprites/blinking_boss_anim.png");
        

        LBitmapFont font;
        LTexture font_texture(renderer);
        string font_path = "lazyfont_blanc.png";
        
        if ( loadMedia(textures, file_path, renderer) and loadFont(&font_texture, &font, font_path))
        {

            
            string layout = "layout2.txt";
            if(classic_level(layout, renderer, textures, font))
                boss_fight(renderer, textures, font);
            
            close(window, renderer, textures);
        }
    }
}
