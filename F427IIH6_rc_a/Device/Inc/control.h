#ifndef __CONTROL_H
#define __CONTROL_H

#include "main.h"
#include "tim.h"
#include "servo.h"
namespace Control
{
    class Control_t
    {
        public:
//       ~Control_t();
        Control_t() = default;
        // Control_t(Servo::Servo_base_t* servoX,Servo): _servoX(&htim4, TIM_CHANNEL_1, 0.0f, 270.0f, 0.0f, 270.0f),_servoY(&htim4, TIM_CHANNEL_2, 0.0f, 270.0f, 0.0f, 270.0f)
        Control_t(Servo::Servo_base_t* servoX,Servo::Servo_base_t* servoY)
        {
            _servoX = servoX;
            _servoY = servoY;
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