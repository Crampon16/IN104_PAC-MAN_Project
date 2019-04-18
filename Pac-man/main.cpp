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
        
        vector<LTexture*> textures;
        vector<string> file_path;
        file_path.push_back("313596.png");
        
        if ( loadMedia(textures, file_path, renderer) )
        {
            cout << "Sprite loading complete." << endl;
            
            //filler code to test the drawing system
            //usefull template for a Stage creator function...
            /*
            Stage stage;
            for (int i = 0; i < STAGE_HEIGHT; ++i)
            {
                vector<Square> temp_vect;
                stage.matrix.push_back(temp_vect);
                for (int j = 0; j < STAGE_WIDTH; ++j)
                {
                    Square temp_square;
                    if ( rand()%3 == 0)
                        temp_square.obstructed = true;
                    else
                        temp_square.item = "gum";
                    
                    stage.matrix[i].push_back(temp_square);
                }
            }
             */
            
            Stage stage = init_stage("layout2.txt");
            Entity pac( {3*SQUARE_SIZE/2, 3*SQUARE_SIZE/2}, 250, std::stack<std::pair<int,int>>() , pacman_AI, 0);
            stage.entities.push_back(pac);
            stage.entities_positions.push_back({1,1});
            
            Entity blinky( {3*SQUARE_SIZE/2, 3*SQUARE_SIZE/2}, 125, std::stack<std::pair<int,int>>() , blinky_AI, 1);
            stage.entities.push_back(blinky);
            stage.entities_positions.push_back({1,1});
            
            cout << SDL_GetTicks() << endl;
            stack<pair<int, int>> path = bfs(stage, {1,1}, {11,11});
            cout << SDL_GetTicks() << endl;
            
            while (not path.empty())
            {
                cout << path.top().first << " " << path.top().second << endl;
                path.pop();
            }
            
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
                    stage.entities[i].move(17, stage);
                }
                display(renderer, stage);
                //cout << "x:" << stage.entities[0].get_position().x << endl;
                //cout << "y:" << stage.entities[0].get_position().y << endl;
                
                cap.cap();
            }
            
            close(window, renderer, textures);
        }
    }
}
