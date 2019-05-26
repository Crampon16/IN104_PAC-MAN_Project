//
//  boss_handler.cpp
//  Pac-man
//


#include "boss_handler.hpp"

using namespace std;

void boss_fight(SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font, vector<Mix_Chunk*> const &sounds, vector<Mix_Music*> const& tracks, bool& victory, bool& quit)
{
    SDL_Event e;
    char key_input = ' ';
    int frames_since_last_bullets = 0;
    
    Avatar pac({BOSS_STAGE_WIDTH/2, 3*BOSS_STAGE_HEIGHT/4}, textures[0]);
    
    vector<LTexture*> boss_textures(textures.end() - 3, textures.end());
    MergeHead mhead({BOSS_STAGE_WIDTH/2, BOSS_STAGE_HEIGHT/4}, boss_textures);
    
    PongRacket prack1({SQUARE_SIZE, BOSS_STAGE_HEIGHT/2}), prack2({BOSS_STAGE_WIDTH - AVATAR_SIZE, BOSS_STAGE_HEIGHT/2});
    PongBall ball({BOSS_STAGE_WIDTH/2, BOSS_STAGE_HEIGHT/2}, {0,0});
    TetrisGrid grid({AVATAR_SIZE,0}, BOSS_STAGE_HEIGHT/AVATAR_SIZE, BOSS_STAGE_WIDTH/AVATAR_SIZE - 2);
    vector<Pellet> pellets;
    
    FPSCapper cap(60);
    
    animation(renderer, textures, font, quit);
    
    bool fight = true;
    while( !quit and fight)
    {
        cap.start();
        while (SDL_PollEvent( &e ) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN and e.key.repeat == false)
            {
                key_input = e.key.keysym.sym;
            }
        }
        
        move(17, pac, key_input, mhead, grid, ball, prack1, prack2, pellets);
        handle_collisions(pac, mhead, grid, ball, pellets, sounds);
        display_stage(renderer, pac, mhead, grid, ball, prack1, prack2, pellets, textures, font);

        if (mhead.get_phase() == BULLET_HELL)
        {
            for (int i = (int)pellets.size() - 1; i >= 0; --i)
            {
                if (pellets[i].is_out_stage())
                {
                    pellets.erase(pellets.begin() + i);
                }
            }
            
            if (frames_since_last_bullets >= 15 and not mhead.is_stunned())
            {
                frames_since_last_bullets = 0;
                SDL_Point boss_pos = mhead.get_position();
                for (int i = 0; i < 8; i++)
                {
                    double angle = i*2*M_PI/8;
                    SDL_Point offset = {static_cast<int>(2*AVATAR_SIZE*cos(angle)), static_cast<int>(2*AVATAR_SIZE*sin(angle))};
                    Pellet pellet({boss_pos.x+offset.x, boss_pos.y+offset.y}, boss_pos);
                    pellets.push_back(pellet);
                }
                
            }
            ++frames_since_last_bullets;
        }
        
        
        if (pac.get_hp() == 0)
        {
            fight = false;
            victory = false;
            //cout << "GAME OVER" << endl;
        }
        
        else if (mhead.get_hp() == 0)
        {
            if (mhead.get_phase() == BULLET_HELL)
            {
                fight = false;
                victory = true;
                
                Mix_HaltMusic();
                Mix_PlayMusic(tracks[3], -1);
                //cout << "YOU WIN!" << endl;
            }
            else
                mhead.next_phase();
        }
        
        cap.cap();
    }
}

void animation(SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font, bool& quit)
{
    SDL_Event e;
    bool animation = true;
    
    FPSCapper cap(60);
    
    Uint32 time_since_animation_beginning = 0;
    while ( !quit and animation )
    {
        cap.start();
        while (SDL_PollEvent( &e ) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);
        
        SDL_Rect clip = {0,0,AVATAR_SIZE, AVATAR_SIZE};
        Uint32 time = SDL_GetTicks()%1000;
        if (time < 250)
            clip.y = 0;
        else if (time < 500)
            clip.y = AVATAR_SIZE;
        else if (time < 750)
            clip.y = 2*AVATAR_SIZE;
        else
            clip.y = 3*AVATAR_SIZE;
        
        if (time_since_animation_beginning < TRANSLATION_PHASE_DURATION)
        {
            int x_offset = BOSS_STAGE_WIDTH/2 * time_since_animation_beginning/TRANSLATION_PHASE_DURATION;
            int y_offset = BOSS_STAGE_HEIGHT/2 * time_since_animation_beginning/TRANSLATION_PHASE_DURATION;
            textures[1]->render({x_offset, y_offset}, &clip);
            textures[2]->render({BOSS_STAGE_WIDTH - x_offset, y_offset}, &clip);
            textures[3]->render({x_offset, BOSS_STAGE_HEIGHT - y_offset}, &clip);
            textures[4]->render({BOSS_STAGE_WIDTH - x_offset, BOSS_STAGE_HEIGHT - y_offset}, &clip);
        }
        
        else if (time_since_animation_beginning < TRANSLATION_PHASE_DURATION + ROTATION_PHASE_DURATION)
        {
            int translational_x_offset = BOSS_STAGE_WIDTH/2;
            int translational_y_offset = BOSS_STAGE_HEIGHT/2 - BOSS_STAGE_HEIGHT*(time_since_animation_beginning - TRANSLATION_PHASE_DURATION)/ROTATION_PHASE_DURATION/4;
            
            double angle = 2*M_PI*(time_since_animation_beginning - TRANSLATION_PHASE_DURATION)/ROTATION_PHASE_DURATION;
            int rotationnal_x_offset = 2*AVATAR_SIZE*cos(angle*angle)*sin(angle/2);
            int rotationnal_y_offset = 2*AVATAR_SIZE*sin(angle*angle)*sin(angle/2);
            
            textures[1]->render({translational_x_offset + rotationnal_x_offset, translational_y_offset+ rotationnal_y_offset}, &clip);
            textures[2]->render({translational_x_offset - rotationnal_y_offset, translational_y_offset + rotationnal_x_offset}, &clip);
            textures[3]->render({translational_x_offset - rotationnal_x_offset, translational_y_offset - rotationnal_y_offset}, &clip);
            textures[4]->render({translational_x_offset + rotationnal_y_offset, translational_y_offset - rotationnal_x_offset}, &clip);
        }
        else
            animation = false;
        
        clip.y = 0;

        textures[0]->render({BOSS_STAGE_WIDTH/2, 3*BOSS_STAGE_HEIGHT/4}, &clip);
        SDL_RenderPresent(renderer);
        
        cap.cap();
        time_since_animation_beginning += 17;
    }
}

void move(Uint32 delta, Avatar& pac, char input, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, PongRacket& r1, PongRacket& r2, vector<Pellet>& pellets)
{
    pac.move(17, input);
    mhead.move(17, pac.get_position());
    
    if (mhead.get_phase() == ANGLE_RAM)
    {
        if (ball.going_left())
        {
            r1.move(17, ball.get_position());
            r2.move(17, {0, BOSS_STAGE_HEIGHT/2});
        }
        else
        {
            r1.move(17, {0, BOSS_STAGE_HEIGHT/2});
            r2.move(17, ball.get_position());
        }
        ball.move(17);
        
        if (ball.out_of_screen())
            ball.serve(pac.get_position());
        if (collide(r1, ball) or collide(r2, ball))
            ball.bounce();
    }
    
    else if (mhead.get_phase() == CONTINUOUS_RAM)
    {
        if (ball.going_left())
        {
            r1.move(17, ball.get_position());
            r2.move(17, {0, BOSS_STAGE_HEIGHT/2});
        }
        else
        {
            r1.move(17, {0, BOSS_STAGE_HEIGHT/2});
            r2.move(17, ball.get_position());
        }
        ball.move(17);
        
        if (ball.out_of_screen())
            ball.serve(pac.get_position());
        if (collide(r1, ball) or collide(r2, ball))
            ball.bounce();
        
        if(rand()%100 < 2)
            grid.add_piece();
        grid.move(17);
    }
    
    else if (mhead.get_phase() == BULLET_HELL)
    {
        for (int i = 0; i < pellets.size(); ++i)
        {
            pellets[i].move(17);
        }
    }
    
    
    
    
}

void handle_collisions(Avatar& pac, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, vector<Pellet>& pellets, vector<Mix_Chunk*> const &sounds)
{
    //update hp
    if (collide(pac, mhead))
    {
        if (mhead.is_stunned())
        {
            mhead.lose_hp(1);
            Mix_PlayChannel( -1, sounds[2], 0 );
        }
        else
        {
            pac.lose_hp(1);
            Mix_PlayChannel( -1, sounds[3], 0 );
        }
    }
    
    if (mhead.get_phase() == ANGLE_RAM)
    {
        if (collide(pac, ball))
        {
            pac.lose_hp(10);
            Mix_PlayChannel( -1, sounds[3], 0 );
        }
    }

    else if (mhead.get_phase() == CONTINUOUS_RAM)
    {
        if (collide(pac, ball))
        {
            pac.lose_hp(1);
            Mix_PlayChannel( -1, sounds[3], 0 );
        }
        
        if (collide(pac, grid))
        {
            pac.lose_hp(1);
            Mix_PlayChannel( -1, sounds[3], 0 );
        }
        
        if (grid.still_blocks_collide(pac))
        {
            pac.lose_hp(20);
            Mix_PlayChannel( -1, sounds[3], 0 );
        }
        
    }
    else if (mhead.get_phase() == BULLET_HELL)
    {
        for (int i = (int)pellets.size() - 1; i >= 0; --i)
        {
            if (collide(pac, pellets[i]))
            {
                pac.lose_hp(2);
                Mix_PlayChannel( -1, sounds[3], 0 );
                pellets.erase(pellets.begin()+i); 
            }
        }
    }
    
    
}

void display_stage(SDL_Renderer* renderer, Avatar& pac, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, PongRacket& r1, PongRacket& r2, vector<Pellet>& pellets, std::vector<LTexture*> const &textures, LBitmapFont& font)
{
    //Clear screen
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0x00 ); //in black
    SDL_RenderClear( renderer );
    
    //Display stage border
    SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xFF, 0x00 );
    SDL_Rect border = {AVATAR_SIZE, 0, BOSS_STAGE_WIDTH - 2*AVATAR_SIZE, BOSS_STAGE_HEIGHT};
    SDL_RenderDrawRect(renderer, &border);
    
    //Render entities
    pac.render(renderer);
    mhead.render(renderer);
    for (int i = 0; i < pellets.size(); ++i)
    {
        pellets[i].render(renderer);
    }
    
    if (mhead.get_phase() == ANGLE_RAM)
    {
        r1.render(renderer);
        r2.render(renderer);
        ball.render(renderer);
    }
    else if (mhead.get_phase() == CONTINUOUS_RAM)
    {
        r1.render(renderer);
        r2.render(renderer);
        ball.render(renderer);
        grid.render(renderer);
    }
    
    
    
    //render colliders
    /*
     pac.render_colliders(renderer);
     mhead.render_colliders(renderer);
     prack1.render_colliders(renderer);
     prack2.render_colliders(renderer);
     ball.render_colliders(renderer);
     */

    //render life bars
    pac.render_hp(renderer);
    font.renderText(225, BOSS_STAGE_HEIGHT - 7, "Player HP");
    mhead.render_hp(renderer);
    font.renderText(225, BOSS_STAGE_HEIGHT + 3*AVATAR_SIZE/2 + 2, "Boss HP");

    
    //Actualize screen
    SDL_RenderPresent(renderer);
}
