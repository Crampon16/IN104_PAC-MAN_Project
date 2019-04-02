//
//  time_wrapper.cpp
//  enstaroid
//
//  Created by Liam Rampon on 20/01/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#include "time_wrapper.hpp"

LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;
    
    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;
    
    //Unpause the timer
    mPaused = false;
    
    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;
    
    //Unpause the timer
    mPaused = false;
    
    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;
        
        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;
        
        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;
        
        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;
    
    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }
    
    return time;
}

bool LTimer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}

FPSCapper::FPSCapper(int maxFps)
{
    fpsTimer = LTimer();
    capTimer = LTimer();
    
    counted_frames = 0;
    max_fps = maxFps;
    tick_per_frame = 1000/max_fps;
    
    fpsTimer.start();
}

void FPSCapper::start()
{
    capTimer.start();
}

void FPSCapper::cap()
{    
    ++counted_frames;
    
    //If frame finished early
    int frameTicks = capTimer.getTicks();
    if( frameTicks < tick_per_frame )
    {
        //Wait remaining time
        SDL_Delay( tick_per_frame - frameTicks );
    }
}

float FPSCapper::get_fps()
{
    float avgFPS = counted_frames / ( fpsTimer.getTicks() / 1000.f );
    if( avgFPS > 2000000 )
    {
        avgFPS = 0;
    }
    
    return avgFPS;
}

