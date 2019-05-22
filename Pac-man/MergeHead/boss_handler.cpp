//
//  boss_handler.cpp
//  Pac-man
//


#include "boss_handler.hpp"

using namespace std;

void boss_fight(SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font)
{
    SDL_Event e;
    char key_input = ' ';
    
    Avatar pac({BOSS_STAGE_WIDTH/2, 3*BOSS_STAGE_HEIGHT/4}, textures[0]);
    
    vector<LTexture*> boss_textures(textures.end() - 3, textures.end());
    MergeHead mhead({BOSS_STAGE_WIDTH/2, BOSS_STAGE_HEIGHT/4}, boss_textures);
    
    PongRacket prack1({SQUARE_SIZE, BOSS_STAGE_HEIGHT/2}), prack2({BOSS_STAGE_WIDTH - AVATAR_SIZE, BOSS_STAGE_HEIGHT/2});
    
    PongBall ball({BOSS_STAGE_WIDTH/2, BOSS_STAGE_HEIGHT/2}, {0,0});
    
    TetrisGrid grid({AVATAR_SIZE,0}, BOSS_STAGE_HEIGHT/AVATAR_SIZE, BOSS_STAGE_WIDTH/AVATAR_SIZE - 2);
    
    vector<Pellet> pellets;
    /*
    SDL_Point center = {BOSS_STAGE_WIDTH/2, BOSS_STAGE_HEIGHT/2};
    for (int i = 0; i < 100; i++)
    {
        Pellet pellet({static_cast<int>(center.x + cos(2*M_PI*i/100.f)), static_cast<int>(center.y + sin(2*M_PI*i/100.f))}, center);
        pellets.push_back(pellet);
    }
     */
    
    FPSCapper cap(60);
    
    bool quit = animation(renderer, textures, font);
    
    while( !quit )
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
        
        move(17, pac, key_input, mhead, grid, ball, prack1, prack2);
        for (int i = 0; i < pellets.size(); ++i)
        {
            pellets[i].move(17);
        }
        handle_collisions(pac, mhead, grid, ball);
        display_stage(renderer, pac, mhead, grid, ball, prack1, prack2, pellets, textures, font);

        
        
        if (pac.get_hp() == 0)
        {
            quit = true;
            cout << "GAME OVER" << endl;
        }
        
        else if (mhead.get_hp() == 0)
        {
            if (mhead.get_phase() == CONTINUOUS_RAM)
            {
                quit = true;
                cout << "YOU WIN!" << endl;
            }
            else
                mhead.next_phase();
        }
        
        cap.cap();
    }
}

bool animation(SDL_Renderer* renderer, std::vector<LTexture*> const &textures, LBitmapFont& font)
{
    SDL_Event e;
    bool quit = false, animation = true;
    
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
    
    return quit;
}

void move(Uint32 delta, Avatar& pac, char input, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, PongRacket& r1, PongRacket& r2)
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
        
        if(rand()%100 == 1)
            grid.add_piece();
        grid.move(17);
    }
    
    
    
}

void handle_collisions(Avatar& pac, MergeHead& mhead, TetrisGrid& grid, PongBall& ball)
{
    //update hp
    if (collide(pac, mhead))
    {
        if (mhead.is_stunned())
            mhead.lose_hp(1);
        else
            pac.lose_hp(1);
    }
    
    if (mhead.get_phase() == ANGLE_RAM)
    {
        if (collide(pac, ball))
            pac.lose_hp(10);
    }

    else if (mhead.get_phase() == CONTINUOUS_RAM)
    {
        if (collide(pac, ball))
            pac.lose_hp(1);
        if (collide(pac, grid))
            pac.lose_hp(1);
        if (grid.still_blocks_collide(pac))
            pac.lose_hp(20);
    }
    
    
}

void display_stage(SDL_Renderer* renderer, Avatar& pac, MergeHead& mhead, TetrisGrid& grid, PongBall& ball, PongRacket& r1, PongRacket& r2, vector<Pellet> pellets, std::vector<LTexture*> const &textures, LBitmapFont& font)
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
