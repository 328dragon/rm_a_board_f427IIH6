#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"
#include "servo.h"
namespace Control
{
    class Control_t
    {
        public:
//       ~Control_t();
        Control_t()
        {
            _x = 0.0f;
            _y = 0.0f;
            _yaw = 0.0f;
        }

        void controlUpdate(float x, float y, float yaw);
        void doCalcontrol();
        void doYawcontrol();


        float _x;
        float _y;
        float _yaw;
        Servo::Servo_base_t* _servoX;
        Servo::Servo_base_t* _servoY;
        Servo::Servo_base_t* _servoYAW;

    };
}
#endif