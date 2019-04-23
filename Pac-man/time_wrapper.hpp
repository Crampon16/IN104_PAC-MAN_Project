//
//  time_wrapper.hpp
//  pac-man
//
//

#ifndef time_wrapper_hpp
#define time_wrapper_hpp

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

class LTimer
{
public:
    //Initializes variables
    LTimer();
    
    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();
    
    //Gets the timer's time
    Uint32 getTicks();
    
    //Checks the status of the timer
    bool isStarted();
    bool isPaused();
    
private:
    //The clock time when the timer started
    Uint32 mStartTicks;
    
    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;
    
    //The timer status
    bool mPaused;
    bool mStarted;
};

class FPSCapper
{
public:
    FPSCapper(int max_fps);
    void start();
    void cap();
    float get_fps();
private:
    LTimer fpsTimer;
    LTimer capTimer;
    
    int counted_frames;
    int max_fps;
    int tick_per_frame;
};


#endif /* time_wrapper_hpp */
