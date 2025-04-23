#include "maincpp.h"
#define PI 3.1415926535
#include "FreeRTOS.h"
#include "task.h"
#include "servo.h"
#include "control.h"
#include "tim.h"



float x=0.0f;
float y=0.0f;
float yaw=0.0f;

#define QUEUE_LENGTH 10
#define ITEM_SIZE sizeof(float)

QueueHandle_t xQueue;


TaskHandle_t Servo1_Motor_handle;
TaskHandle_t Servo2_Motor_handle;
TaskHandle_t Remote_control_handle;
// TaskHandle_t Message_update_handle;
Control::Control_t control;



void remoteControl(void *pvParameters);
void OnServo1_Control(void *pvParameters);
void OnServo2_Control(void *pvParameters);
// void message_update(void *pvParameters);
void main_cpp(void)
{
xQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);
control._servoX = new Servo::Servo_base_t(&htim4, TIM_CHANNEL_1,0.0f, 270.0f, 0.0f, 270.0f);
control._servoY = new Servo::Servo_base_t(&htim4, TIM_CHANNEL_2,0.0f, 270.0f, 0.0f, 270.0f);
BaseType_t ok2=xTaskCreate(OnServo1_Control,"Server1_Motor",600,NULL,3,&Servo1_Motor_handle);
BaseType_t ok3=xTaskCreate(OnServo2_Control,"Server2_Motor",600,NULL,3,&Servo2_Motor_handle);
BaseType_t ok4=xTaskCreate(remoteControl,"remote_contorl",1000,NULL,5,&Remote_control_handle);
// BaseType_t ok5=xTaskCreate(message_update,"Message_update",600,NULL,6,&Message_update_handle);

if(ok2!=pdPASS||ok3!=pdPASS||ok4!=pdPASS)
{
	 while(1)
	 {}
}	

}

// void message_update(void *pvParameters)
// {

// }

void remoteControl(void *pvParameters)
{
  while(1)
  {
  
  control.controlUpdate(x,y,yaw);

	  vTaskDelay(50);
  }


}

void OnServo1_Control(void *pvParameters)
{
  
  while(1)
  {
    control.doCalcontrol();   
    vTaskDelay(200);
  }


}

void OnServo2_Control(void *pvParameters)
{
  while(1)
  {
    control.doYawcontrol();
      vTaskDelay(200);
  }

}

extern "C"
{

#ifdef __MICROLIB
#include <stdio.h>

  int fputc(int ch, FILE *f)
  {
    (void)f;
    (void)ch;

    return ch;
  }
#else
#include <rt_sys.h>

  FILEHANDLE $Sub$$_sys_open(const char *name, int openmode)
  {
    (void)name;
    (void)openmode;
    return 0;
  }
#endif

  void _sys_exit(int ret)
  {
    (void)ret;
    while (1)
    {
    }
  }
  void _ttywrch(int ch)
  {
    (void)ch;
  }
}