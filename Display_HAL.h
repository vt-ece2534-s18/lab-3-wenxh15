/*
 * Display_HAL.h
 *
 *  Created on: Mar 13, 2018
 *      Author: temp
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdint.h>

#define MY_BLACK GRAPHICS_COLOR_BLACK
#define MY_WHITE GRAPHICS_COLOR_WHITE


void InitGraphics();
void LCDClearDisplay(int color);
void LCDDrawChar(unsigned row, unsigned col, char c);
void PrintString(char *str, int row, int col);



#endif /* DISPLAY_H_ */
