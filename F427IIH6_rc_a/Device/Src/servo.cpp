#include "servo.h"
#include "FreeRTOS.h"
#include "task.h"
using namespace Servo;

void Servo_base_t::initServo()
{
    HAL_TIM_PWM_Start(_htim, _channel);
    __HAL_TIM_SET_COMPARE(_htim, _channel, 0);
}

void Servo_base_t::control()
{
    uint32_t pulse = 0;
    if (_target_angle >= _current_angle)
    {

        uint32_t step = (_target_angle - _current_angle) / 10.0f;
        for (uint32_t i = _current_angle; i <= _target_angle; i += step)
        {
            pulse = (i * 4) / 3.0f;
            __HAL_TIM_SET_COMPARE(_htim, _channel, pulse);
            vTaskDelay(_velocity);
        }
    }
    else
    {
        uint32_t step = (_current_angle - _target_angle) / 10.0f;
        for (uint32_t i = _current_angle; i >= _target_angle; i -= step)
        {
            pulse = (i * 4) / 3.0f;
            __HAL_TIM_SET_COMPARE(_htim, _channel, pulse);
            vTaskDelay(_velocity);
        }
    }

    _current_angle = _target_angle;
}
