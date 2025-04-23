#ifndef __SERVO_H
#define __SERVO_H
#include "main.h"
//#include "task.h"
namespace Servo 
{
    class Servo_base_t
    {
    public:
        Servo_base_t()=default;
        Servo_base_t(TIM_HandleTypeDef *htim, uint32_t channel,float min_angle,float max_angle,float use_min_angle,float use_max_angle)
        {
            _htim = htim;
            _channel = channel;
            _target_angle = 0.0f;
            _current_angle=_target_angle;
            _velocity = 0.0f;
            _min_angle=min_angle;
            _max_angle=max_angle;
            _use_min_angle=use_min_angle;
            _use_max_angle=use_max_angle;
        }

//        ~ServoMotor();

        void initServo();
        void control(float angle);

        float _target_angle;
        float _current_angle;
        float _velocity;
        float _min_angle;
        float _max_angle;
        float _use_min_angle;
        float _use_max_angle;

    protected:
        TIM_HandleTypeDef *_htim;
        uint32_t _channel;
    };
}// namespace Servo


#endif 
