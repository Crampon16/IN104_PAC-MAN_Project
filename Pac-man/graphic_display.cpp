//
//  graphic_display.cpp
//  Pac-man
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

void display(SDL_Renderer* renderer, Stage stage, LBitmapFont& font, vector<LTexture*> const &textures)
{
    //Clear screen
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 ); //in black
    //SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0x00 ); //in white
    SDL_RenderClear( renderer );
    
    
    //Draw the limits of the stage OR NOT !
    /*
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xFF, 0xFF ); //Draw in blue
    SDL_Rect stage_outline = {0,0, SQUARE_SIZE*STAGE_WIDTH, SQUARE_SIZE*STAGE_HEIGHT};
    SDL_RenderDrawRect(renderer, &stage_outline);
    */
    
    
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
                for (int clip_index = 0; clip_index < stage.matrix[i][j].sprites.size(); ++clip_index)
                {
                    SDL_Rect clip = {stage.matrix[i][j].sprites[clip_index].x, stage.matrix[i][j].sprites[clip_index].y, AVATAR_SIZE, AVATAR_SIZE};
                    textures[WALL_ID]->render({square_center.x - AVATAR_SIZE/2, square_center.y - AVATAR_SIZE/2}, &clip);
                }
            }
            /*
             if (stage.matrix[i][j].is_node)
             {
             SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF ); //Draw in green
             SDL_RenderDrawRect(renderer, &square_outline);
             }
             */
            
            
            
            if (stage.matrix[i][j].item == "gum")
            {
                SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF ); //Draw in yellow
                DrawCircle(renderer, square_center, SQUARE_SIZE/5);
            }
            if (stage.matrix[i][j].item == "super_gum")
            {
                SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0xFF, 0xFF ); //Draw in yellow
                DrawCircle(renderer, square_center, SQUARE_SIZE/5);
            }
            square_outline.x += SQUARE_SIZE;
            square_center.x += SQUARE_SIZE;
        }
        square_outline.y += SQUARE_SIZE;
        square_center.y += SQUARE_SIZE;
    }
    
    
    //Draw the entities in the stage
    /*
     SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF ); //Draw in yellow
     DrawCircle(renderer, stage.entities[pac_man_id].get_position(), SQUARE_SIZE/2);
     */
    
    Uint32 current_time = SDL_GetTicks()%1000;
    
    //for pacman
    SDL_Point sprite_pos = stage.entities[pac_man_id].get_position();
    sprite_pos.x -= SQUARE_SIZE/2;
    sprite_pos.y -= SQUARE_SIZE/2;
    
    SDL_Rect src;
    src.x = SQUARE_SIZE;
    src.y = 0;
    src.h = SQUARE_SIZE;
    src.w = SQUARE_SIZE;
    
    if(current_time < 250)
        src.y = 0;
    else if(current_time < 500)
        src.y = SQUARE_SIZE;
    else if(current_time < 750)
        src.y = SQUARE_SIZE*2;
    else
        src.y = SQUARE_SIZE*3;
    
    pair<int, int> direction;
    direction.first =  stage.entities[pac_man_id].get_path().top().first - stage.entities[pac_man_id].get_previous_square().first;
    direction.second =  stage.entities[pac_man_id].get_path().top().second - stage.entities[pac_man_id].get_previous_square().second;
    
    bool moving = false;
    
    switch (direction.first)
    {
        case 1 :
            src.x *= 3;
            moving = true;
            break;
        case -1 :
            src.x *= 1;
            moving = true;
            break;
        default : break;
    }
    switch (direction.second)
    {
        case 1 :
            src.x *= 0;
            moving = true;
            break;
        case -1 :
            src.x *= 2;
            moving = true;
            break;
        default : break;
    }
    
    if (!moving)
        src.y = 0;
    
    textures[pac_man_id]->render(sprite_pos, &src);
    
    //for ghosts
    for (int i = 1; i <= 4; ++i)
    {
        
        SDL_Point sprite_pos = stage.entities[i].get_position();
        sprite_pos.x -= SQUARE_SIZE/2;
        sprite_pos.y -= SQUARE_SIZE/2;
        
        //textures[pac_man_id]->render(sprite_pos);
        
        SDL_Rect src;
        src.x = SQUARE_SIZE;
        src.y = 0;
        src.h = SQUARE_SIZE;
        src.w = SQUARE_SIZE;
        
        if(current_time < 250)
            src.y = 0;
        else if(current_time < 500)
            src.y = SQUARE_SIZE;
        else if(current_time < 750)
            src.y = SQUARE_SIZE*2;
        else
            src.y = SQUARE_SIZE*3;
        
        pair<int, int> direction;
        direction.first =  stage.entities[i].get_path().top().first - stage.entities[i].get_previous_square().first;
        direction.second =  stage.entities[i].get_path().top().second - stage.entities[i].get_previous_square().second;
        
        
        switch (direction.first)
        {
            case 1 : src.x *= 3; break;
            case -1 : src.x *= 1; break;
            default : break;
        }
        switch (direction.second)
        {
            case 1 : src.x *= 0; break;
            case -1 : src.x *= 2; break;
            default : break;
        }
        
        if (stage.entities[i].state == AFRAID)
        {
            if (SDL_GetTicks() - stage.killer_mode_start < 3500)
                textures[AFRAID_ID]->render(sprite_pos, &src);
            else
                textures[BLINKING_ID]->render(sprite_pos, &src);
        }
        else if (stage.entities[i].state == DEAD)
        {
            src.y = 0;
            textures[DEAD_ID]->render(sprite_pos, &src);
        }
        else
        {
            textures[i]->render(sprite_pos, &src);
        }
        
        
    }
    
    /*
     SDL_SetRenderDrawColor( renderer, 0x88, 0x88, 0xFF, 0xFF ); //Draw in light blue
     DrawCircle(renderer, stage.entities[1].get_position(), SQUARE_SIZE/2);
     SDL_SetRenderDrawColor( renderer, 0xFF, 0x40, 0x40, 0xFF ); //Draw in pink
     DrawCircle(renderer, stage.entities[2].get_position(), SQUARE_SIZE/2);
     SDL_SetRenderDrawColor( renderer, 0xFF, 0x40, 0x40, 0xFF ); //Draw in pink
     DrawCircle(renderer, stage.entities[3].get_position(), SQUARE_SIZE/2);
     SDL_SetRenderDrawColor( renderer, 0xFF, 0x40, 0x40, 0xFF ); //Draw in pink
     DrawCircle(renderer, stage.entities[4].get_position(), SQUARE_SIZE/2);
     */
    
    //Show score and remaining lives
    string score = "Score: " + to_string(stage.score);
    string lives = "Lives: " + to_string(stage.lives);
    font.renderText(0, SCREEN_HEIGHT - 4*SQUARE_SIZE - 10, score);
    font.renderText(0, SCREEN_HEIGHT - 2*SQUARE_SIZE - 10, lives);
    
    
    SDL_RenderPresent( renderer );
}
