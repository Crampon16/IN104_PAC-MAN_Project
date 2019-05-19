//
//  entity_rebirth.hpp
//  Pac-man
//
//  Created by Liam Rampon on 27/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef entity_rebirth_hpp
#define entity_rebirth_hpp

#include <stdio.h>
#include <vector>

#include "SDL2/SDL.h"
#include "texture_wrapper.hpp"

#include "struct_and_enum.hpp"

#include "constants.hpp"



class EntityRebirth
{
public:
    EntityRebirth(SDL_Point pos, std::vector<SDL_Rect> colli);
    void move();
    
    SDL_Point get_position();
    std::vector<SDL_Rect> const& get_colliders();
    
    friend bool collide(EntityRebirth& entA, EntityRebirth& entB);
    
    virtual void render(SDL_Renderer*) = 0;
    void render_colliders(SDL_Renderer* renderer);
    
    //void render(SDL_Renderer* renderer);
    
protected:
    Float_Point position;
    
    int speed; //in pxl/s
    Float_Point movement_direction;

    std::vector<SDL_Rect> colliders;
};

class Avatar: public EntityRebirth
{
public:
    Avatar(SDL_Point pos, LTexture* text);
    void move(Uint32 delta, char key_press);
    
    void lose_hp(int loss);
    int get_hp();
    
    
    void render(SDL_Renderer* renderer);
    void render_hp(SDL_Renderer* renderer);
    
private:
    int hp;
    LTexture* texture;

};

class MergeHead: public EntityRebirth
{
public:
    MergeHead(SDL_Point pos, std::vector<LTexture*> text);
    /*
     MergeHead (メージヘッド）has four phases of movement and a stun phase.
     
     First phase
     - it first rumbles around its position (0.5s)
     - it then rams in the direction of the player's position at the end of the rumble (2s)
     - this is repeated 3 times, then MergeHead is stunned (5s)
     
     Second phase (PongBall and PongRacket appear)
     - it first rumbles around its position (0.5s)
     - it then rams in the direction of the player's position at the end of the rumble (2s)
     - it immediatly rams again towards the player (2s)
     - this is repeated 3 times, then MergeHead is stunned (5s)
     
     Third phase (Tetrominos appear)
     - it first rumbles around its position (0.5s)
     - it rams 5 five decreasingly long times in a row towards the player (1s + 800ms + 600ms + 400ms + 200ms)
     - this is repeated 3 times, then MergeHead is stunned (5s)
     
     Fourth (other ennemies disappear)
     - スカルレットオバドリヴ!!!
    */
    void move(Uint32 delta, SDL_Point avatar_pos);
    
    bool is_stunned();
    bool is_rumbling();
    
    void next_phase();
    Phase get_phase();
    
    //will decrease hp by loss points, and do nothing if invulnerable is true
    void lose_hp(int loss);
    int get_hp();

    void render(SDL_Renderer* renderer);
    void render_hp(SDL_Renderer* renderer);

    
private:
    int hp;
    
    bool invulnerable;
    
    Uint32 time_since_subphase_beginning;
    Phase phase;
    SubPhase subphase;
    int phase_repeat;
    
    void retarget(SDL_Point pos);
    void bounce();
    
    std::vector<LTexture*> textures;

};

class TetrisBlock: public EntityRebirth
{
public:
    TetrisBlock(SDL_Point pos, char shape);
    void move(Uint32 delta);
    
    std::vector<int> get_bottom_squares();
    void render(SDL_Renderer* renderer);

private:
    std::vector<int> bottom_squares;
};

class PongRacket: public EntityRebirth
{
public:
    PongRacket(SDL_Point pos);
    void move(Uint32 delta, SDL_Point ball_pos);
    void render(SDL_Renderer* renderer);

};

class PongBall: public EntityRebirth
{
public:
    PongBall(SDL_Point pos, SDL_Point targ);
    
    void move(Uint32 delta);
    void bounce();
    
    bool going_left();
    bool out_of_screen();
    void serve(SDL_Point target);
    void render(SDL_Renderer* renderer);

    
private:
};

#endif /* entity_rebirth_hpp */
