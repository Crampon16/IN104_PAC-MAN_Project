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
        *window = SDL_CreateWindow( "Overworld", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

int main(int argc, const char * argv[])
{

    SDL_Window* window;
    SDL_Renderer* renderer;
    
    if( init(&window, &renderer) )
    {
        cout << "Initialization complete." << endl;
        
        vector<LTexture*> textures;
        vector<string> file_path;
        
        if ( loadMedia(textures, file_path, renderer) )
        {
            cout << "Sprite loading complete." << endl;
            
            SDL_Event e;
            bool quit = false;
            
            while( !quit )
            {
                while (SDL_PollEvent( &e ) != 0)
                {
                    if(e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }
            }
            
            close(window, renderer, textures);
        }
    }
}
