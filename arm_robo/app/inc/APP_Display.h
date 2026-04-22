#ifndef __APP_DISPLAY_H
#define __APP_DISPLAY_H

#include <stdint.h>
#include "MAX7219.h"
#include "DSA.h"

void Display_Init(void);
void Display_ShowError(void);
void Display_ShowMode1(void);
void Display_ShowMode2(void);
void Display_ShowStop(void);

#endif