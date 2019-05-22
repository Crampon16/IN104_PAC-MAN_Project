//
//  stage_rebirth.hpp
//  Pac-man
//


#ifndef stage_reborn_hpp
#define stage_reborn_hpp

#include <stdio.h>

#include <vector>
#include <queue>
#include <algorithm>

#include "SDL2/SDL.h"
#include "texture_wrapper.hpp"

#include "struct_and_enum.hpp"
#include "entity_rebirth.hpp"

#include "constants.hpp"

class TetrisGrid: public EntityRebirth
{
    
public:
    TetrisGrid(SDL_Point pos, int height, int width);
    
    //if the piece to create intersects with static blocks, the grid is reset and the piece created
    //if the piece to create intersects with moving blocks, it isn't created
    void add_piece(char shape = ' ', int column = -1);
    void reset();
    
    void move(Uint32 delta);
    
    bool still_blocks_collide(EntityRebirth& ent);
    
    void render(SDL_Renderer* renderer);
    
private:
    
    int height, width;
    
    //[0][0] is on the upper-left-hand corner of the screen
    std::vector<std::vector<bool>> filled_squares;
    
    //each element is the index of the last empty square of the column
    //if the column is full, the index is -1
    std::vector<int> upper_limit;
    
    std::vector<std::vector<std::pair<int, int>>> moving_pieces;
    
    void actualize_colliders();
    
    Uint32 time_since_last_update;
    
    //int speed (inherithed from EntityRebirth) is that of the falling blocks
    
};

#endif /* stage_reborn_hpp */
