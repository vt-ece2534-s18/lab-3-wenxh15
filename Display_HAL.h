//------------------------------------------
// DISPLAY API (Application Programming Interface)
// Also known as DISPLAY HAL (Hardware Abstraction Layer)
// HAL is a specific form of API that designs the interface with a certain hardware

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <ti/grlib/grlib.h>
#include <stdlib.h>


#define MY_BLACK GRAPHICS_COLOR_BLACK
#define MY_WHITE GRAPHICS_COLOR_WHITE


void InitGraphics();
void LCDClearDisplay(int color);
void LCDDrawChar(unsigned row, unsigned col, int8_t c);
void LCDDrawLine();
void LCDDrawCar(int16_t prevCarX, int16_t curCarX);
void LCDDrawObs(int16_t prevObs, int16_t curObs, int16_t randObs);
void LCDDrawBonus(int16_t prevObs, int16_t curObs, int16_t randObs);


void PrintString(char *str, int row, int col);
void ClearString(char *str, int row, int col);



#endif /* DISPLAY_H_ */
