#ifndef __APP_H
#define __APP_H

#include "APP_Control_Robot.h"
#include "APP_Read_Button.h"


// GPIO TEST
void GPIO_Test_Polling_Live(void);

// Robot_Control
void Arm_Init(void);
void Arm_Start(void);
void Arm_Stop(void);

//Display
void Display_Init(void);
void Display_ShowError(void);
void Display_ShowMode1(void);
void Display_ShowMode2(void);
void Display_ShowStop(void);


#endif