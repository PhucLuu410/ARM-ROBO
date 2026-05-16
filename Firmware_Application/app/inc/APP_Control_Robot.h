#ifndef __CONTROL_ROBOT_H
#define __CONTROL_ROBOT_H

#include <stdint.h>
#include <math.h>

#include "DSA.h"
#include "BSP.h"

void Arm_Init(void);
void Arm_Start(void);
void Arm_Mode1(void);
void Arm_Mode2(void);
void Arm_Stop(void);
void Arm_OffSet(void);

#endif
