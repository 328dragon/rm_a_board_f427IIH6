#include "maincpp.h"
#define PI 3.1415926535
#include "FreeRTOS.h"
#include "task.h"
#include "servo.h"
#include "control.h"
#include "tim.h"

float x = 0.0f;
float y = 0.0f;
float yaw = 0.0f;

#define QUEUE_LENGTH 10
#define ITEM_SIZE sizeof(float)

QueueHandle_t xQueue;

TaskHandle_t Omain_handle;
TaskHandle_t Servo1_Motor_handle;
TaskHandle_t Servo2_Motor_handle;
TaskHandle_t Remote_control_handle;

Servo::Servo_base_t servoX;
Servo::Servo_base_t servoY;
Control::Control_t control;

void Onmain_Task(void *pvParameters);
void remoteControl(void *pvParameters);
void OnServo1_Control(void *pvParameters);
void OnServo2_Control(void *pvParameters);
// void message_update(void *pvParameters);
void main_cpp(void)
{
  servoX = Servo::Servo_base_t(&htim4, TIM_CHANNEL_1, 0.0f, 0.0f, 270.0f, 0.0f, 180.0f);
  servoY = Servo::Servo_base_t(&htim4, TIM_CHANNEL_2, 0.0f, 0.0f, 270.0f, 0.0f, 180.0f);
  control = Control::Control_t(&servoX, &servoY);
  xQueue = xQueueCreate(QUEUE_LENGTH, ITEM_SIZE);
  BaseType_t ok1 = xTaskCreate(Onmain_Task, "Onmain_Task", 600, NULL, 6, &Omain_handle);
  BaseType_t ok2 = xTaskCreate(OnServo1_Control, "Servo1_Motor", 1000, NULL, 3, &Servo1_Motor_handle);
  BaseType_t ok3 = xTaskCreate(OnServo2_Control, "Servo2_Motor", 1000, NULL, 3, &Servo2_Motor_handle);
  BaseType_t ok4 = xTaskCreate(remoteControl, "remote_contorl", 500, NULL, 5, &Remote_control_handle);

  if (ok2 != pdPASS || ok3 != pdPASS || ok4 != pdPASS)
  {
    while (1)
    {
    }
  }
}

void Onmain_Task(void *pvParameters)
{
  while (1)
  {
    vTaskDelay(50);
  }
}

void remoteControl(void *pvParameters)
{
  while (1)
  {

    control.controlUpdate(x, y, yaw);

    vTaskDelay(50);
  }
}

void OnServo1_Control(void *pvParameters)
{

  while (1)
  {
    servoX.control();
    vTaskDelay(200);
  }
}

void OnServo2_Control(void *pvParameters)
{
  while (1)
  {
    servoY.control();
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