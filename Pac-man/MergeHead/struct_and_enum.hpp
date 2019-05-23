//
//  struct_and_enum.h
//  Pac-man
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

static std::vector<std::vector<std::pair<int, int>>> shapes =
{    
    { {0,0}, {0,1}, {0,2}, {1,0} }, //L
    { {0,0}, {1,0}, {2,0}, {2,1} },
    { {1,0}, {1,1}, {1,2}, {0,2} },
    { {0,0}, {0,1}, {1,1}, {2,1} },
    
    
    { {0,0}, {0,1}, {0,2}, {1,2} }, //reverse L
    { {0,1}, {1,1}, {2,1}, {2,0} },
    { {1,0}, {1,1}, {1,2}, {0,0} },
    { {0,0}, {1,0}, {2,0}, {0,1} },
    
    
    { {0,0}, {1,0}, {2,0}, {3,0} }, //I
    { {0,0}, {0,1}, {0,2}, {0,3} },
    
    
    { {0,0}, {0,1}, {0,2}, {1,1} }, //T
    { {1,0}, {1,1}, {1,2}, {0,1} },
    { {0,0}, {1,0}, {2,0}, {1,1} },
    { {1,0}, {0,1}, {1,1}, {2,1} },
    
    
    { {0,1}, {0,2}, {1,0}, {1,1} }, //S
    { {0,0}, {1,0}, {1,1}, {2,1} },
    
    { {0,0}, {0,1}, {1,1}, {1,2} }, // reverse S
    { {1,0}, {2,0}, {0,1}, {1,1} }
};



#endif /* struct_and_enum_h */
