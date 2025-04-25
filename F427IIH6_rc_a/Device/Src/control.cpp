#include "control.h"
#include "math.h"
using namespace Control;

void Control::Control_t::controlUpdate(float x, float y, float yaw)
{
    _x = x/100.0f;
    _y = y/100.0f;
    _yaw = yaw/100.0f;

    (_servoX)->_target_angle =2*atan((_y-sqrt(-(_x*_x+_y*_y)*(25*_x*_x+25*_y*_y-1)))/(5*_x*_x-_x+5*_y*_y));
    (_servoY)->_target_angle=-2*atan((_y-sqrt(-(_x*_x+_y*_y)*(25*_x*_x+25*_y*_y-1)))/(5*_x*_x+_x+5*_y*_y));
    
}
