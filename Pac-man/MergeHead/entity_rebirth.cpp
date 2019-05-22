//
//  entity_rebirth.cpp
//  Pac-man
//


#include "entity_rebirth.hpp"

using namespace std;

//constructors
EntityRebirth::EntityRebirth(SDL_Point pos, std::vector<SDL_Rect> colli)
{
    position.x = pos.x;
    position.y = pos.y;
    movement_direction = {0,0};
    
    colliders = colli;
}
Avatar::Avatar(SDL_Point pos,  LTexture* text):EntityRebirth(pos, {{0, 0, AVATAR_SIZE, AVATAR_SIZE}})
{
    speed = BOSS_STAGE_WIDTH/2;
    
    hp = 100;
    
    texture = text;
}
MergeHead::MergeHead(SDL_Point pos, vector<LTexture*> text):EntityRebirth(pos, {{0, 0, 2*AVATAR_SIZE, 2*AVATAR_SIZE}})
{
    speed = BOSS_STAGE_WIDTH;
    
    time_since_subphase_beginning = 0;
    phase = NORMAL_RAM;
    subphase = RUMBLE;
    
    phase_repeat = 0;
    
    hp = 100;
    
    textures = text;
    frame_number = 0;
}
TetrisBlock::TetrisBlock(SDL_Point pos, char shape):EntityRebirth(pos, {})
{
    switch (shape) {
        case 'l':
            colliders.push_back({ 0, 0, AVATAR_SIZE, AVATAR_SIZE });
            colliders.push_back({ 0, -AVATAR_SIZE,AVATAR_SIZE,AVATAR_SIZE });
            colliders.push_back({ AVATAR_SIZE, 0, AVATAR_SIZE, AVATAR_SIZE });
            colliders.push_back({ 2*AVATAR_SIZE, 0, AVATAR_SIZE, AVATAR_SIZE });
            colliders.push_back({ 3*AVATAR_SIZE, 0, AVATAR_SIZE, AVATAR_SIZE });
            
            bottom_squares = {0, 0, 0, 0};
            break;
            
        case 'i':
            colliders.push_back({ 0, 0, AVATAR_SIZE, AVATAR_SIZE });
            colliders.push_back({ 0, -AVATAR_SIZE,AVATAR_SIZE,AVATAR_SIZE });
            colliders.push_back({ 0, -2*AVATAR_SIZE, AVATAR_SIZE, AVATAR_SIZE });
            colliders.push_back({ 0, -3*AVATAR_SIZE, AVATAR_SIZE, AVATAR_SIZE });
            
            bottom_squares = {0};
            break;
        
        case 't':
            colliders.push_back({ 0, 0, AVATAR_SIZE, AVATAR_SIZE });
            colliders.push_back({ 0, -AVATAR_SIZE,AVATAR_SIZE,AVATAR_SIZE });
            colliders.push_back({ AVATAR_SIZE, -AVATAR_SIZE, AVATAR_SIZE, AVATAR_SIZE });
            colliders.push_back({ -AVATAR_SIZE, -AVATAR_SIZE, AVATAR_SIZE, AVATAR_SIZE });
            
            bottom_squares = {0,1,0};
        default:
            break;
    }
    
    speed = BOSS_STAGE_WIDTH/3;
    movement_direction = {0,1};
}
PongRacket::PongRacket(SDL_Point pos):EntityRebirth(pos, {{0,0,AVATAR_SIZE,4*AVATAR_SIZE}})
{
    speed = BOSS_STAGE_HEIGHT/3;
}
PongBall::PongBall(SDL_Point pos, SDL_Point targ):EntityRebirth(pos, {{0,0,AVATAR_SIZE/2,AVATAR_SIZE/2}})
{
    speed = BOSS_STAGE_WIDTH/2;
    
    movement_direction = {targ.x - position.x, targ.y - position.y};
    float norm = sqrt(movement_direction.x*movement_direction.x + movement_direction.y*movement_direction.y);
    movement_direction.x /= norm;
    movement_direction.y /= norm;
}
Pellet::Pellet(SDL_Point pos, SDL_Point targ):EntityRebirth(pos, {{0,0, AVATAR_SIZE/2, AVATAR_SIZE/2}})
{
    speed = BOSS_STAGE_HEIGHT;
    
    movement_direction = {targ.x - position.x, targ.y - position.y};
    float norm = sqrt(movement_direction.x*movement_direction.x + movement_direction.y*movement_direction.y);
    movement_direction.x /= norm;
    movement_direction.y /= norm;
    
    out_of_stage = false;
}


//movers
void Avatar::move(Uint32 delta, char key_press)
{
    int signed_delta = delta;
    switch (key_press)
    {
        case 'z':
            movement_direction = {0,-1};
            break;
        case 'q':
            movement_direction = {-1,0};
            break;
        case 's':
            movement_direction = {0,1};
            break;
        case 'd':
            movement_direction = {1,0};
            break;
            
        default:
            break;
    }
    position.x += movement_direction.x*speed*signed_delta/1000.f;
    position.y += movement_direction.y*speed*signed_delta/1000.f;
    
    if (position.x < 3*AVATAR_SIZE/2)
        position.x = 3*AVATAR_SIZE/2;
    if (position.x > BOSS_STAGE_WIDTH - 3*AVATAR_SIZE/2)
        position.x = BOSS_STAGE_WIDTH - 3*AVATAR_SIZE/2;
    if (position.y < AVATAR_SIZE/2)
        position.y = AVATAR_SIZE/2;
    if (position.y > BOSS_STAGE_HEIGHT - AVATAR_SIZE/2)
        position.y = BOSS_STAGE_HEIGHT - AVATAR_SIZE/2;
}
void MergeHead::move(Uint32 delta, SDL_Point avatar_pos)
{
    time_since_subphase_beginning += delta;
    
    switch (phase) {
        case NORMAL_RAM:
        {
            if (subphase == RUMBLE)
            {
                //if time has come to begin ramming
                if (time_since_subphase_beginning > 500)
                {
                    //change subphase
                    subphase = RAM;
                    time_since_subphase_beginning = 0;
                    
                    //recalculate target
                    retarget(avatar_pos);
                    
                    //end invulnerability time (activated by a phase change)
                    invulnerable = false;
                }
            }
            else if(subphase == RAM)
            {
                int signed_delta = delta;
                position.x += movement_direction.x*speed*signed_delta/1000.f;
                position.y += movement_direction.y*speed*signed_delta/1000.f;
                
                //bounce if an edge has been reached
                if (position.x < 2*AVATAR_SIZE or position.x > BOSS_STAGE_WIDTH - 2*AVATAR_SIZE)
                    movement_direction.x *= -1;
                if (position.y < AVATAR_SIZE or position.y > BOSS_STAGE_HEIGHT - AVATAR_SIZE)
                    movement_direction.y *= -1;


                
                //if time has come to begin rumbling
                if (time_since_subphase_beginning > 2000)
                {
                    //change subphase
                    subphase = RUMBLE;
                    time_since_subphase_beginning = 0;
                    
                    ++phase_repeat;
                    
                    if (phase_repeat == 3)
                    {
                        subphase = STUN;
                        phase_repeat = 0;
                    }
                }
            }
            else if (subphase == STUN)
            {
                if (time_since_subphase_beginning > 5000)
                {
                    time_since_subphase_beginning = 0;
                    subphase = RUMBLE;
                }
            }
            break;
        }
        
        case ANGLE_RAM:
        {
            if (subphase == RUMBLE)
            {
                //if time has come to begin ramming
                if (time_since_subphase_beginning > 500)
                {
                    //change subphase
                    subphase = RAM;
                    time_since_subphase_beginning = 0;
                    
                    //recalculate target
                    retarget(avatar_pos);
                    
                    //end invulnerability time (activated by a phase change)
                    invulnerable = false;
                }
            }
            else if(subphase == RAM)
            {
                int signed_delta = delta;
                position.x += movement_direction.x*speed*signed_delta/1000.f;
                position.y += movement_direction.y*speed*signed_delta/1000.f;
                
                //bounce if an edge has been reached
                if (position.x < 2*AVATAR_SIZE or position.x > BOSS_STAGE_WIDTH - 2*AVATAR_SIZE)
                    movement_direction.x *= -1;
                if (position.y < AVATAR_SIZE or position.y > BOSS_STAGE_HEIGHT - AVATAR_SIZE)
                    movement_direction.y *= -1;
                
                
                //if time has come to make an angle turn
                if (time_since_subphase_beginning > 2000)
                {
                    //change subphase
                    subphase = SECOND_RAM;
                    time_since_subphase_beginning = 0;
                    
                    //recalculate target
                    retarget(avatar_pos);
                }
            }
            else if(subphase == SECOND_RAM)
            {
                int signed_delta = delta;
                position.x += movement_direction.x*speed*signed_delta/1000.f;
                position.y += movement_direction.y*speed*signed_delta/1000.f;
                
                //bounce if an edge has been reached
                if (position.x < 2*AVATAR_SIZE or position.x > BOSS_STAGE_WIDTH - 2*AVATAR_SIZE)
                    movement_direction.x *= -1;
                if (position.y < AVATAR_SIZE or position.y > BOSS_STAGE_HEIGHT - AVATAR_SIZE)
                    movement_direction.y *= -1;
                
                
                //if time has come to begin rumbling
                if (time_since_subphase_beginning > 2000)
                {
                    //change subphase
                    subphase = RUMBLE;
                    time_since_subphase_beginning = 0;
                    
                    ++phase_repeat;
                    
                    if (phase_repeat == 3)
                    {
                        subphase = STUN;
                        phase_repeat = 0;
                    }

                }
            }
            else if (subphase == STUN)
            {
                if (time_since_subphase_beginning > 5000)
                {
                    time_since_subphase_beginning = 0;
                    subphase = RUMBLE;
                }
            }
            break;
        }
            
        case CONTINUOUS_RAM:
        {
            if (subphase == RUMBLE)
            {                
                //if time has come to begin ramming
                if (time_since_subphase_beginning > 500)
                {
                    //change subphase
                    subphase = RAM;
                    time_since_subphase_beginning = 0;
                    
                    //recalculate target
                    retarget(avatar_pos);
                    
                    //end invulnerability time (activated by a phase change)
                    invulnerable = false;
                }
            }
            else if(subphase == RAM)
            {
                int signed_delta = delta;
                position.x += movement_direction.x*speed*signed_delta/1000.f;
                position.y += movement_direction.y*speed*signed_delta/1000.f;
                
                //bounce if an edge has been reached
                if (position.x < 2*AVATAR_SIZE or position.x > BOSS_STAGE_WIDTH - 2*AVATAR_SIZE)
                    movement_direction.x *= -1;
                if (position.y < AVATAR_SIZE or position.y > BOSS_STAGE_HEIGHT - AVATAR_SIZE)
                    movement_direction.y *= -1;
                
                
                //if time has come to change phase
                if (time_since_subphase_beginning > 1000)
                {
                    //change subphase
                    subphase = SECOND_RAM;
                    time_since_subphase_beginning = 0;
                    
                    //recalculate target
                    retarget(avatar_pos);
                }
            }
            else if(subphase == SECOND_RAM)
            {
                int signed_delta = delta;
                position.x += movement_direction.x*speed*signed_delta/1000.f;
                position.y += movement_direction.y*speed*signed_delta/1000.f;
                
                //bounce if an edge has been reached
                if (position.x < 2*AVATAR_SIZE or position.x > BOSS_STAGE_WIDTH - 2*AVATAR_SIZE)
                    movement_direction.x *= -1;
                if (position.y < AVATAR_SIZE or position.y > BOSS_STAGE_HEIGHT - AVATAR_SIZE)
                    movement_direction.y *= -1;
                
                
                //if time has come to change phase
                if (time_since_subphase_beginning > 800)
                {
                    //change subphase
                    subphase = THIRD_RAM;
                    time_since_subphase_beginning = 0;
                    
                    //recalculate target
                    retarget(avatar_pos);
                }
            }
            else if(subphase == THIRD_RAM)
            {
                int signed_delta = delta;
                position.x += movement_direction.x*speed*signed_delta/1000.f;
                position.y += movement_direction.y*speed*signed_delta/1000.f;
                
                //bounce if an edge has been reached
                if (position.x < 2*AVATAR_SIZE or position.x > BOSS_STAGE_WIDTH - 2*AVATAR_SIZE)
                    movement_direction.x *= -1;
                if (position.y < AVATAR_SIZE or position.y > BOSS_STAGE_HEIGHT - AVATAR_SIZE)
                    movement_direction.y *= -1;
                
                
                //if time has come to change phase
                if (time_since_subphase_beginning > 600)
                {
                    //change subphase
                    subphase = FOURTH_RAM;
                    time_since_subphase_beginning = 0;
                    
                    //recalculate target
                    retarget(avatar_pos);
                }
            }
            else if(subphase == FOURTH_RAM)
            {
                int signed_delta = delta;
                position.x += movement_direction.x*speed*signed_delta/1000.f;
                position.y += movement_direction.y*speed*signed_delta/1000.f;
                
                //bounce if an edge has been reached
                if (position.x < 2*AVATAR_SIZE or position.x > BOSS_STAGE_WIDTH - 2*AVATAR_SIZE)
                    movement_direction.x *= -1;
                if (position.y < AVATAR_SIZE or position.y > BOSS_STAGE_HEIGHT - AVATAR_SIZE)
                    movement_direction.y *= -1;
                
                
                //if time has come to change phase
                if (time_since_subphase_beginning > 400)
                {
                    //change subphase
                    subphase = FIFTH_RAM;
                    time_since_subphase_beginning = 0;
                    
                    //recalculate target
                    retarget(avatar_pos);
                }
            }
            else if(subphase == FIFTH_RAM)
            {
                int signed_delta = delta;
                position.x += movement_direction.x*speed*signed_delta/1000.f;
                position.y += movement_direction.y*speed*signed_delta/1000.f;
                
                //bounce if an edge has been reached
                if (position.x < 2*AVATAR_SIZE or position.x > BOSS_STAGE_WIDTH - 2*AVATAR_SIZE)
                    movement_direction.x *= -1;
                if (position.y < AVATAR_SIZE or position.y > BOSS_STAGE_HEIGHT - AVATAR_SIZE)
                    movement_direction.y *= -1;
                
                
                //if time has come to rumble
                if (time_since_subphase_beginning > 200)
                {
                    //change subphase
                    subphase = RUMBLE;
                    time_since_subphase_beginning = 0;
                    
                    ++phase_repeat;
                    
                    if (phase_repeat == 3)
                    {
                        subphase = STUN;
                        phase_repeat = 0;
                    }

                }
            }
            else if (subphase == STUN)
            {
                if (time_since_subphase_beginning > 5000)
                {
                    time_since_subphase_beginning = 0;
                    subphase = RUMBLE;
                }
            }

            break;
        }
            
        case BULLET_HELL:
        default:
            break;

    }
}
void TetrisBlock::move(Uint32 delta)
{
    int signed_delta = delta;
    position.y += movement_direction.y*signed_delta*speed/1000.f;
}
void PongRacket::move(Uint32 delta, SDL_Point ball_pos)
{
    //small error margin: the Racket won't move if close enough
    if(ball_pos.y - position.y > 10)
        movement_direction.y = 1;
    else if(ball_pos.y - position.y < -10)
        movement_direction.y = -1;
    else
        movement_direction.y = 0;
        
    int signed_delta = delta;
    position.y += movement_direction.y*signed_delta*speed/1000.f;
}
void PongBall::move(Uint32 delta)
{
    int signed_delta = delta;
    position.x += movement_direction.x*speed*signed_delta/1000.f;
    position.y += movement_direction.y*speed*signed_delta/1000.f;
    
    //bounce if the upper of lower edge has been reached
    if (position.y < 0 or position.y > BOSS_STAGE_HEIGHT)
        movement_direction.y *= -1;
}
void Pellet::move(Uint32 delta)
{
    int signed_delta = delta;
    position.x += movement_direction.x*speed*signed_delta/1000.f;
    position.y += movement_direction.y*speed*signed_delta/1000.f;
    
    //register if gone out of the stage
    if (position.y < 0 or position.y > BOSS_STAGE_HEIGHT or position.x < 0 or position.x > BOSS_STAGE_WIDTH)
        out_of_stage = true;
}

//rendering
void Avatar::render(SDL_Renderer *renderer)
{
    /*
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0x00, 0xFF ); //in yellow
    SDL_Rect pac_rect = {static_cast<int>(position.x - AVATAR_SIZE/2), static_cast<int>(position.y - AVATAR_SIZE/2), AVATAR_SIZE, AVATAR_SIZE};
    SDL_RenderFillRect(renderer, &pac_rect);
    */
    
    SDL_Rect clip = {0,0, AVATAR_SIZE, AVATAR_SIZE};
    
    Uint32 time = SDL_GetTicks() % 1000;
    if (time < 333)
        clip.y = 0;
    else if(time < 666)
        clip.y = AVATAR_SIZE;
    else
        clip.y = 2*AVATAR_SIZE;
    
    if (movement_direction.x > 0)
        clip.x = 0;
    else if (movement_direction.y < 0)
        clip.x = AVATAR_SIZE;
    else if (movement_direction.x < 0)
        clip.x = 2*AVATAR_SIZE;
    else
        clip.x = 3*AVATAR_SIZE;
    
    
    texture->render({static_cast<int>(position.x - AVATAR_SIZE/2), static_cast<int>(position.y - AVATAR_SIZE/2)}, &clip);
}
void Avatar::render_hp(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
    SDL_Rect remaining_life = {AVATAR_SIZE/2, BOSS_STAGE_HEIGHT + AVATAR_SIZE/2, (10*AVATAR_SIZE*hp)/100, AVATAR_SIZE};
    SDL_RenderFillRect(renderer, &remaining_life);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_Rect lost_life = {AVATAR_SIZE/2 + (10*AVATAR_SIZE*hp)/100, BOSS_STAGE_HEIGHT + AVATAR_SIZE/2, (10*AVATAR_SIZE*(100-hp))/100, AVATAR_SIZE};
    SDL_RenderFillRect(renderer, &lost_life);
}
void MergeHead::render(SDL_Renderer *renderer)
{
    SDL_Rect clip = {0,0, 2*AVATAR_SIZE, 2*AVATAR_SIZE};
    Uint32 time = SDL_GetTicks() % 1000;
    
    int sprite = 0;
    if ( subphase == STUN )
    {
        if (time < 250)
            clip.y = 0;
        else if (time < 500)
            clip.y = 2*AVATAR_SIZE;
        else if (time < 750)
            clip.y = 4*AVATAR_SIZE;
        else
            clip.y = 6*AVATAR_SIZE;
        
        if (time_since_subphase_beginning < 3500)
            sprite = 1;
        else
            sprite = 2;
    }
    else
    {
        clip.y = frame_number* 2*AVATAR_SIZE;
        frame_number = (frame_number+1) % (textures[0]->getHeight()/(2*AVATAR_SIZE));
    }
    
    if (movement_direction.x > 0)
        clip.x = 0;
    else if (movement_direction.y < 0)
        clip.x = 2*AVATAR_SIZE;
    else if (movement_direction.x < 0)
        clip.x = 4*AVATAR_SIZE;
    else
        clip.x = 6*AVATAR_SIZE;
    
    if(subphase == RUMBLE) //random movements if rumbling
        textures[sprite]->render({static_cast<int>(position.x - AVATAR_SIZE) + (rand()%10 - 5), static_cast<int>(position.y - AVATAR_SIZE + (rand()%10 - 5))}, &clip);
    else
        textures[sprite]->render({static_cast<int>(position.x - AVATAR_SIZE), static_cast<int>(position.y - AVATAR_SIZE)}, &clip);
}
void MergeHead::render_hp(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
    SDL_Rect remaining_life = {AVATAR_SIZE/2, BOSS_STAGE_HEIGHT + 5*AVATAR_SIZE/2, (10*AVATAR_SIZE*hp)/100, AVATAR_SIZE};
    SDL_RenderFillRect(renderer, &remaining_life);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
    SDL_Rect lost_life = {AVATAR_SIZE/2 + (10*AVATAR_SIZE*hp)/100, BOSS_STAGE_HEIGHT + 5*AVATAR_SIZE/2, (10*AVATAR_SIZE*(100-hp))/100, AVATAR_SIZE};
    SDL_RenderFillRect(renderer, &lost_life);
}
void TetrisBlock::render(SDL_Renderer *renderer)
{
    
}
void PongRacket::render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF ); // in white
    SDL_Rect rack_rect = {static_cast<int>(position.x - AVATAR_SIZE/2), static_cast<int>(position.y - 2*AVATAR_SIZE), AVATAR_SIZE, 4*AVATAR_SIZE};
    SDL_RenderFillRect(renderer, &rack_rect);
}
void PongBall::render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF ); // in white
    SDL_Rect ball_rect = {static_cast<int>(position.x - AVATAR_SIZE/4), static_cast<int>(position.y - AVATAR_SIZE/4), AVATAR_SIZE/2, AVATAR_SIZE/2};
    SDL_RenderFillRect(renderer, &ball_rect);
}
void Pellet::render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor( renderer, rand()%255, rand()%255, rand()%255, 0xFF ); // in RAINBOW
    SDL_Rect pellet_rect = {static_cast<int>(position.x - AVATAR_SIZE/4), static_cast<int>(position.y - AVATAR_SIZE/4), AVATAR_SIZE/2, AVATAR_SIZE/2};
    SDL_RenderFillRect(renderer, &pellet_rect);
}


//misc
SDL_Point EntityRebirth::get_position()
{
    int x = position.x, y = position.y;
    return {x, y};
}

bool MergeHead::is_stunned()
{
    return subphase == STUN;
}
bool MergeHead::is_rumbling()
{
    return subphase == RUMBLE;
}
void MergeHead::retarget(SDL_Point pos)
{
    //calculate direction of movement
    movement_direction = {pos.x - position.x, pos.y - position.y};
    float norm = sqrt(movement_direction.x*movement_direction.x + movement_direction.y*movement_direction.y);
    movement_direction.x /= norm;
    movement_direction.y /= norm;
}
void MergeHead::next_phase()
{
    hp = 100;
    invulnerable = true;
    if (phase == NORMAL_RAM)
        phase = ANGLE_RAM;
    else if (phase == ANGLE_RAM)
        phase = CONTINUOUS_RAM;
    else if (phase == CONTINUOUS_RAM)
        phase = BULLET_HELL;
}

void PongBall::bounce()
{
    movement_direction.x *= -1;
}
void PongBall::serve(SDL_Point target)
{
    //replace at the center of the stage
    position = {BOSS_STAGE_WIDTH/2, BOSS_STAGE_HEIGHT/2};
    
    //calculate direction of movement
    movement_direction = {target.x - position.x, target.y - position.y};
    float norm = sqrt(movement_direction.x*movement_direction.x + movement_direction.y*movement_direction.y);
    movement_direction.x /= norm;
    movement_direction.y /= norm;
}
bool PongBall::out_of_screen()
{
    return position.x < 0 or BOSS_STAGE_WIDTH < position.x;
}
bool PongBall::going_left()
{
    return movement_direction.x < 0;
        
}

vector<int> TetrisBlock::get_bottom_squares()
{
    return bottom_squares;
}

void Avatar::lose_hp(int loss)
{
    hp -= loss;
    if (hp < 0)
        hp = 0;
}
int Avatar::get_hp()
{
    return hp;
}
void MergeHead::lose_hp(int loss)
{
    if (not invulnerable)
    {
        hp -= loss;
        if (hp < 0)
            hp = 0;
    }
    
}
int MergeHead::get_hp()
{
    return hp;
}
Phase MergeHead::get_phase()
{
    return phase;
}

bool Pellet::is_out_stage()
{
    return out_of_stage;
}


//collision detection
std::vector<SDL_Rect>const& EntityRebirth::get_colliders()
{
    return colliders;
}
//[S] Collide.
bool collide(EntityRebirth & entA, EntityRebirth & entB)
{
    SDL_Rect hitboxA, hitboxB;
    SDL_Point offsetA = entA.get_position(), offsetB = entB.get_position();
    
    for (int i = 0; i < entA.colliders.size(); ++i)
        for (int j = 0; j < entB.colliders.size(); ++j)
        {
            hitboxA = entA.colliders[i];
            hitboxA.x += offsetA.x - hitboxA.w/2;
            hitboxA.y += offsetA.y - hitboxA.h/2;
            
            hitboxB = entB.colliders[j];
            hitboxB.x += offsetB.x - hitboxB.w/2;
            hitboxB.y += offsetB.y - hitboxB.h/2;
            
            if(SDL_HasIntersection(&hitboxA, &hitboxB))
                return true;
        }
    return false;
}

void EntityRebirth::render_colliders(SDL_Renderer *renderer)
{
    SDL_Rect hitbox;
    SDL_Point offset = get_position();
    
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF); //draw in green
    for (int i = 0; i < colliders.size(); ++i)
    {
        hitbox = colliders[i];
        hitbox.x += offset.x - hitbox.w/2;
        hitbox.y += offset.y - hitbox.h/2;
        SDL_RenderDrawRect(renderer, &hitbox);
    }
}
