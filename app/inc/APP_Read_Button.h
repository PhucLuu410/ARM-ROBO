#ifndef __READ_BUTTON_H
#define __READ_BUTTON_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "DSA.h"
#include "BSP.h"

void Button_Init(void);

void Button_Pressed(void);
void Emergency_Button_Pressed(void);

#endif