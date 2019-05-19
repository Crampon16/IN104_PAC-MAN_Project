//
//  struct_and_enum.h
//  Pac-man
//
//  Created by Liam Rampon on 27/04/2019.
//  Copyright © 2019 Liam Rampon. All rights reserved.
//

#ifndef struct_and_enum_h
#define struct_and_enum_h

#include <vector>

struct Float_Point
{
    float x = 0,y = 0;
};

enum Phase
{
    NORMAL_RAM,
    ANGLE_RAM,
    CONTINUOUS_RAM,
    BULLET_HELL,
};

enum SubPhase
{
    RUMBLE, RAM, SECOND_RAM, THIRD_RAM, FOURTH_RAM, FIFTH_RAM, STUN
};

class SquareToFill
{
public:
    SquareToFill(int c, int l, Uint32 time){
    column = c;
    line = l;
    fill_date = time;
}
    bool operator<(SquareToFill square_a){
        return fill_date < square_a.fill_date;
    }
    
    int column, line;
    Uint32 fill_date;
};

static std::vector<std::vector<std::pair<int, int>>> shapes = {
    { {0,0}, {1,0}, {0,1}, {0,2}, {0,3} }, //L
    { {0,0}, {1,0}, {2,0}, {3,0} }, //I
    { {0,0}, {0,1}, {0,2}, {1,1} } //T    
};



#endif /* struct_and_enum_h */
