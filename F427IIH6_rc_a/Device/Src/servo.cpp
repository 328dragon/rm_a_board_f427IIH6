#include "servo.h"
#include "FreeRTOS.h"
#include "task.h"
using namespace Servo;
#define rad_to_degree 57.30f
#define rate_to_pulse 3.7f





void Servo_base_t::control(float vel)
{
	_velocity=100;
    float pulse = 0;
	float step=0;
	float temp_angle=0;
	step=(_target_angle>_current_angle)? ((_target_angle - _current_angle)/5.0f): ((_current_angle - _target_angle) / 5.0f);
    if (_target_angle > _current_angle)
    {

        for (temp_angle = _current_angle; temp_angle <= _target_angle; temp_angle += step)
        {
            pulse = (int)((temp_angle * 3.7f)+250);
            __HAL_TIM_SET_COMPARE(_htim, _channel, pulse);
            vTaskDelay(_velocity);
        }   
				   _current_angle = _target_angle;
    }
    else if(_target_angle < _current_angle)
    {
//       step = (_current_angle - _target_angle) / 10.0f;
        for (temp_angle = _current_angle; temp_angle >= _target_angle;temp_angle-= step)
        {
             pulse = (int)((temp_angle * 3.7f)+250);
            __HAL_TIM_SET_COMPARE(_htim, _channel, pulse);
            vTaskDelay(_velocity);
        }
				   _current_angle = _target_angle;
    }




}
