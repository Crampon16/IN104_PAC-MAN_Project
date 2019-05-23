//
//  stage_rebirth.cpp
//  Pac-man
//


#include "stage_reborn.hpp"

using namespace std;

TetrisGrid::TetrisGrid(SDL_Point pos, int h, int w):EntityRebirth(pos, {})
{
    height = h;
    width = w;
    
    for (int i = 0; i < height; ++i)
    {
        filled_squares.push_back({});
        for (int j = 0; j < width; ++j)
        {
            filled_squares[i].push_back(false);
        }
    }
    for (int i = 0; i < width; ++i)
        upper_limit.push_back(height-1);
    
    speed = 4*AVATAR_SIZE;
    movement_direction = {0,1};
    
    time_since_last_update = 0;
}

void TetrisGrid::add_piece(int column)
{
    /*if (shape == ' ')
    {
        switch (rand()%3)
        {
            case 0:
                add_piece('l', column);
                break;
            case 1:
                add_piece('i', column);
                break;
            case 2:
                add_piece('t', column);
                break;
                
            default:
                break;
        }
    }*/
    
    if (column == -1)
        column = rand()%width;
    
    /*switch (shape)
    {

        case 'l':
        {
            if (column > width - 3) //if column is out of the grid to the right, replace it rightmost
                column = width - 3;
            
            else if (column < 0) //if column is out of the grid to the left, replace it leftmost
                column = 0;
            
            //if the piece is intersecting with a filled square at its creation, reset the grid
            if (filled_squares[1][column] or filled_squares[0][column] or filled_squares[1][column + 1] or filled_squares[1][column + 2] or filled_squares[1][column + 3])
                reset();
            
            bool intersects_moving = false;
            //if the piece is intersecting with a moving piece at its creation, do not create it
            for (int p = 0; p < moving_pieces.size(); ++p)
            {
                vector<pair<int, int>>& piece = moving_pieces[p];
                for (int b = 0; b < piece.size(); ++b)
                {
                    pair<int, int> block = piece[b];
                    block.second -= column;
                    if (find(shapes[0].begin(), shapes[0].end(), block) != shapes[0].end())
                    {
                        intersects_moving = true;
                        break;
                    }
                }
            }
            
            if (not intersects_moving)
                moving_pieces.push_back( { {1, column}, {0, column}, {0, column + 1}, {0, column + 2} });

            break;
        }
            
        case 'i':
        {
            if (column > width - 1) //if column is out of the grid to the right, replace it rightmost
                column = width - 1;
            
            else if (column < 0) //if column is out of the grid to the left, replace it leftmost
                column = 0;
            
            //test if the piece isn't intersecting with anything at its creation.
            if (filled_squares[0][column] or filled_squares[1][column] or filled_squares[2][column] or filled_squares[3][column])
                reset();
            
            bool intersects_moving = false;
            //if the piece is intersecting with a moving piece at its creation, do not create it
            for (int p = 0; p < moving_pieces.size(); ++p)
            {
                vector<pair<int, int>>& piece = moving_pieces[p];
                for (int b = 0; b < piece.size(); ++b)
                {
                    pair<int, int> block = piece[b];
                    block.second -= column;
                    if (find(shapes[1].begin(), shapes[1].end(), block) != shapes[1].end())
                    {
                        intersects_moving = true;
                        break;
                    }
                }
            }
            
            if (not intersects_moving)
                moving_pieces.push_back({ {0, column}, {1, column}, {2, column}, {3, column} });
            
            break;
        }
            
        case 't':
        {
            if (column > width - 3) //if column is out of the grid to the right, replace it rightmost
                column = width - 3;
            
            else if (column < 0) //if column is out of the grid to the left, replace it leftmost
                column = 0;
            
            //test if the piece isn't intersecting with anything at its creation.
            if (filled_squares[0][column] or filled_squares[0][column + 1] or filled_squares[0][column + 2] or filled_squares[1][column + 1])
                reset();
            
            bool intersects_moving = false;
            //if the piece is intersecting with a moving piece at its creation, do not create it
            for (int p = 0; p < moving_pieces.size(); ++p)
            {
                vector<pair<int, int>>& piece = moving_pieces[p];
                for (int b = 0; b < piece.size(); ++b)
                {
                    pair<int, int> block = piece[b];
                    block.second -= column;
                    if (find(shapes[2].begin(), shapes[2].end(), block) != shapes[2].end())
                    {
                        intersects_moving = true;
                        break;
                    }
                }
            }
            
            if (not intersects_moving)
                moving_pieces.push_back({ {0, column}, {0, column + 1}, {0, column + 2}, {1, column + 1} });
            
            break;
            
        }
            
        default:
            break;
    }*/
    
    int shape_number = rand()%shapes.size();
    
    int piece_width = shapes[shape_number][0].second;
    for (int i = 1; i < shapes[shape_number].size(); ++i)
        if (shapes[shape_number][i].second > piece_width)
            piece_width = shapes[shape_number][i].second;
    ++piece_width;
    
    if (column > width - piece_width) //if column is out of the grid to the right, replace it rightmost
        column = width - piece_width;
    
    else if (column < 0) //if column is out of the grid to the left, replace it leftmost
        column = 0;
    
    //test if the piece isn't intersecting with anything at its creation.
    for (int i = 0; i < shapes[shape_number].size(); ++i)
        if (filled_squares[shapes[shape_number][i].first][column+shapes[shape_number][i].second])
        {
            reset();
            break;
        }
    
    bool intersects_moving = false;
    //if the piece is intersecting with a moving piece at its creation, do not create it
    for (int p = 0; p < moving_pieces.size(); ++p)
    {
        vector<pair<int, int>>& piece = moving_pieces[p];
        for (int b = 0; b < piece.size(); ++b)
        {
            pair<int, int> block = piece[b];
            block.second -= column;
            if (find(shapes[shape_number].begin(), shapes[shape_number].end(), block) != shapes[shape_number].end())
            {
                intersects_moving = true;
                break;
            }
        }
    }
    
    if (not intersects_moving)
    {
        vector<pair<int, int>> piece;
        for (int i = 0; i < shapes[shape_number].size(); ++i)
            piece.push_back({shapes[shape_number][i].first, shapes[shape_number][i].second + column});
        
        moving_pieces.push_back(piece);
    }
    
    
    
    actualize_colliders();
    
}

void TetrisGrid::reset()
{
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            filled_squares[i][j] = false;
    
    moving_pieces.erase(moving_pieces.begin(), moving_pieces.end());
}

void TetrisGrid::move(Uint32 delta)
{
    time_since_last_update += delta;
    
    if( (float)(time_since_last_update*speed) / 1000.f > AVATAR_SIZE)
    {
        //iterate on each piece (e.g. L, I, T...)
        for (int p =  (int)moving_pieces.size() -1 ; p >= 0; --p)
        {
            vector<pair<int, int>>& piece = moving_pieces[p];
            bool reached_bottom = false;
            
            //look, for each block in the piece, if it reaches the bottom line
            for (int b = 0; b < piece.size(); ++b)
            {
                pair<int, int>& block = piece[b];
                if (block.first == height - 1) // test this first to avoid bad access with next test
                {
                    reached_bottom = true;
                    break;
                }
                if (filled_squares[block.first + 1][block.second])
                {
                    reached_bottom = true;
                    break;
                }
            }
            
            if (reached_bottom)
            {
                for (int b = 0; b < piece.size(); ++b)
                {
                    pair<int, int> block = piece[b];
                    filled_squares[block.first][block.second] = true;
                    upper_limit[block.second] = min(block.first - 1, upper_limit[block.second]);
                }
                moving_pieces.erase(moving_pieces.begin() + p);
            }
            else
                for (int b = 0; b < piece.size(); ++b)
                    ++piece[b].first;
        }
        time_since_last_update = 0;
    }
    
    actualize_colliders();
}

void TetrisGrid::render(SDL_Renderer *renderer)
{
    SDL_Rect display_rect = {static_cast<int>(position.x), static_cast<int>(position.y), AVATAR_SIZE, AVATAR_SIZE};
    
    //display still blocks
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (filled_squares[i][j])
            {
                //render the inside of each square in grey
                SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80, 0xFF);
                SDL_RenderFillRect(renderer, &display_rect);
                //render the outline of the square in white
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderDrawRect(renderer, &display_rect);
                //render a red cross to warn of the instakill danger
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0x00);
                SDL_RenderDrawLine(renderer, display_rect.x, display_rect.y, display_rect.x + display_rect.w, display_rect.y + display_rect.h);
                SDL_RenderDrawLine(renderer, display_rect.x + display_rect.w, display_rect.y, display_rect.x, display_rect.y + display_rect.h);
                
            }
            display_rect.x += AVATAR_SIZE;
        }
        display_rect.x -= AVATAR_SIZE*width;
        display_rect.y += AVATAR_SIZE;
    }
    
    //display moving blocks
    for (int p = 0; p < moving_pieces.size(); ++p)
    {
        vector<pair<int, int>>& piece = moving_pieces[p];
        for (int b = 0; b < piece.size(); ++b)
        {
            pair<int, int>& block = piece[b];
            display_rect.x = position.x + AVATAR_SIZE*block.second;
            display_rect.y = position.y + AVATAR_SIZE*block.first;
            
            //inside in bright red
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(renderer, &display_rect);
            //outline in a darker shade
            SDL_SetRenderDrawColor(renderer, 0x80, 0x00, 0x00, 0xFF);
            SDL_RenderDrawRect(renderer, &display_rect);
        }
    }

}

void TetrisGrid::actualize_colliders()
{
    colliders.erase(colliders.begin(), colliders.end());
    
    //iterate on each piece (e.g. L, I, T...)
    for (int p = 0; p < moving_pieces.size(); ++p)
    {
        vector<pair<int, int>>& piece = moving_pieces[p];
        for (int b = 0; b < piece.size(); ++b)
        {
            pair<int, int>& block = piece[b];
            SDL_Rect collider = {static_cast<int>(position.x + block.second*AVATAR_SIZE), static_cast<int>(position.y + block.first*AVATAR_SIZE), AVATAR_SIZE, AVATAR_SIZE};
            colliders.push_back(collider);
        }
    }
}

bool TetrisGrid::still_blocks_collide(EntityRebirth& ent)
{
    SDL_Rect hitboxA, hitboxB;
    SDL_Point offsetA = ent.get_position() ;
    
    std::vector<SDL_Rect> const& extern_colliders = ent.get_colliders();
    
    for (int i = 0; i < extern_colliders.size(); ++i)
        for (int column = 0; column < upper_limit.size(); ++column)
        for (int line = upper_limit[column]; line < filled_squares.size(); ++line)
        {
            if (not filled_squares[line][column])
                continue;
            
            hitboxA = extern_colliders[i];
            hitboxA.x += offsetA.x - hitboxA.w/2;
            hitboxA.y += offsetA.y - hitboxA.h/2;
            
            hitboxB = {column*AVATAR_SIZE, line*AVATAR_SIZE, AVATAR_SIZE, AVATAR_SIZE};
            
            if(SDL_HasIntersection(&hitboxA, &hitboxB))
                return true;
        }
    return false;
}
