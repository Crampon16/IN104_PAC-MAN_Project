//
//  graphic_display.cpp
//  Pac-man
//
//  Created by Liam Rampon on 03/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "graphic_display.hpp"

using namespace std;

//this function courtesy of https://stackoverflow.com/questions/38334081/howto-draw-circles-arcs-and-vector-graphics-in-sdl
typedef int32_t s32;
void DrawCircle(SDL_Renderer *Renderer, SDL_Point center, s32 radius)
{
    s32 _x = center.x, _y = center.y;

    s32 x = radius - 1;
    s32 y = 0;
    s32 tx = 1;
    s32 ty = 1;
    s32 err = tx - (radius << 1); // shifting bits left by 1 effectively
    // doubles the value. == tx - diameter
    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(Renderer, _x + x, _y - y);
        SDL_RenderDrawPoint(Renderer, _x + x, _y + y);
        SDL_RenderDrawPoint(Renderer, _x - x, _y - y);
        SDL_RenderDrawPoint(Renderer, _x - x, _y + y);
        SDL_RenderDrawPoint(Renderer, _x + y, _y - x);
        SDL_RenderDrawPoint(Renderer, _x + y, _y + x);
        SDL_RenderDrawPoint(Renderer, _x - y, _y - x);
        SDL_RenderDrawPoint(Renderer, _x - y, _y + x);

        if (err <= 0)
        {
            y++;
            err += ty;
            ty += 2;
        }
        if (err > 0)
        {
            x--;
            tx += 2;
            err += tx - (radius << 1);
        }
    }
}

void display(SDL_Renderer* renderer, Stage stage, LBitmapFont& font)
{
    //Clear screen
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 ); //in black
    //SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0x00 ); //in white
    SDL_RenderClear( renderer );


    //Draw the limits of the stage
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xFF, 0xFF ); //Draw in blue
    SDL_Rect stage_outline = {0,0, SQUARE_SIZE*STAGE_WIDTH, SQUARE_SIZE*STAGE_HEIGHT};
    SDL_RenderDrawRect(renderer, &stage_outline);


    //Draw each Square of the stage
    SDL_Rect square_outline = {0, 0, SQUARE_SIZE, SQUARE_SIZE};
    SDL_Point square_center = {SQUARE_SIZE/2, SQUARE_SIZE/2};

    for (int i = 0; i < STAGE_HEIGHT; ++i)
    {
        square_outline.x = 0;
        square_center.x = SQUARE_SIZE/2;

        for (int j = 0; j < STAGE_WIDTH; ++j)
        {
            if (stage.matrix[i][j].obstructed)
            {
                SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xFF, 0xFF ); //Draw in blue
                SDL_RenderFillRect(renderer, &square_outline);
            }
            if (stage.matrix[i][j].is_node)
            {
                SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF ); //Draw in green
                SDL_RenderDrawRect(renderer, &square_outline);
            }



            if (stage.matrix[i][j].item == "gum")
            {
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF ); //Draw in yellow
                DrawCircle(renderer, square_center, SQUARE_SIZE/5);
            }
            if (stage.matrix[i][j].item == "super_gum")
            {
                SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0xFF, 0xFF ); //Draw in purple
                DrawCircle(renderer, square_center, SQUARE_SIZE/5);
            }
            square_outline.x += SQUARE_SIZE;
            square_center.x += SQUARE_SIZE;
        }
        square_outline.y += SQUARE_SIZE;
        square_center.y += SQUARE_SIZE;
    }


    //Draw the entities in the stage
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF ); //Draw in yellow
    DrawCircle(renderer, stage.entities[0].get_position(), SQUARE_SIZE/2);
    SDL_SetRenderDrawColor( renderer, 0x88, 0x88, 0xFF, 0xFF ); //Draw in light blue
    DrawCircle(renderer, stage.entities[1].get_position(), SQUARE_SIZE/2);
    SDL_SetRenderDrawColor( renderer, 0xFF, 0x40, 0x40, 0xFF ); //Draw in pink
    DrawCircle(renderer, stage.entities[2].get_position(), SQUARE_SIZE/2);

    //Show score and remaining lives
    string score = "Score: " + to_string(stage.score);
    string lives = "Lives: " + to_string(stage.lives);
    font.renderText(0, SCREEN_HEIGHT - 4*SQUARE_SIZE - 10, score);
    font.renderText(0, SCREEN_HEIGHT - 2*SQUARE_SIZE - 10, lives);


    SDL_RenderPresent( renderer );
}
