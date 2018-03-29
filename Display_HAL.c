#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

Graphics_Context g_sContext;

void InitGraphics() {
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext,
                         &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    GrContextFontSet(&g_sContext, &g_sFontCmtt16);
    Graphics_clearDisplay(&g_sContext);
}

void LCDClearDisplay(int color) {
    Graphics_setBackgroundColor(&g_sContext, color);
    Graphics_clearDisplay(&g_sContext);
}


void LCDDrawChar(unsigned row, unsigned col, int8_t c) {
    Graphics_drawString(&g_sContext,
                        &c,
                        1,
                        8 * (col % 16),
                        16 * (row % 8),
                        OPAQUE_TEXT);
}

void LCDDrawLine()
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawLineV(&g_sContext,14,0,128);
    Graphics_drawLineV(&g_sContext,13,0,128);
    Graphics_drawLineV(&g_sContext,64,0,128);
    Graphics_drawLineV(&g_sContext,63,0,128);

}

void LCDDrawCar(int16_t prevCarX, int16_t curCarX)
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_Rectangle R;
    R.xMin = prevCarX;
    R.xMax = prevCarX+15;
    R.yMin = 112;
    R.yMax = 127;
    Graphics_fillRectangle(&g_sContext, &R);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    R.xMin = curCarX;
    R.xMax = curCarX+15;
    R.yMin = 112;
    R.yMax = 127;
    Graphics_fillRectangle(&g_sContext, &R);
}

void LCDDrawObs(int16_t ObsX, int16_t prevObsY, int16_t curObsY)
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_Rectangle R;
    R.xMin = ObsX;//15
    R.xMax = ObsX+15;
    R.yMin = prevObsY;
    R.yMax = prevObsY + 15;
    Graphics_fillRectangle(&g_sContext, &R);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    R.xMin = ObsX;
    R.xMax = ObsX+15;
    R.yMin = curObsY;
    R.yMax = curObsY+15;
    Graphics_fillRectangle(&g_sContext, &R);
}

void LCDDrawObs1(int16_t prevObs1, int16_t curObs1)
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_Rectangle R;
    R.xMin = 15;
    R.xMax = 30;
    R.yMin = prevObs1;
    R.yMax = prevObs1 + 15;
    Graphics_fillRectangle(&g_sContext, &R);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    R.xMin = 15;
    R.xMax = 130;
    R.yMin = curObs1;
    R.yMax = curObs1+15;
    Graphics_fillRectangle(&g_sContext, &R);
}

void LCDDrawObs2(int16_t prevObs2, int16_t curObs2)
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_Rectangle R;
    R.xMin = 31;
    R.xMax = 46;
    R.yMin = prevObs2;
    R.yMax = prevObs2+15;
    Graphics_fillRectangle(&g_sContext, &R);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    R.xMin = 31;
    R.xMax = 46;
    R.yMin = curObs2;
    R.yMax = curObs2+15;
    Graphics_fillRectangle(&g_sContext, &R);
}

void LCDDrawObs3(int16_t prevObs3, int16_t curObs3)
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_Rectangle R;
    R.xMin = 47;
    R.xMax = 62;
    R.yMin = prevObs3;
    R.yMax = prevObs3+15;
    Graphics_fillRectangle(&g_sContext, &R);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    R.xMin = 47;
    R.xMax = 62;
    R.yMin = curObs3;
    R.yMax = curObs3+15;
    Graphics_fillRectangle(&g_sContext, &R);
}

void PrintString(char *str, int row, int col) {

    int i;
    for (i = 0; str[i] != '\0'; i++) {
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_GREEN);
        LCDDrawChar(row,  col, str[i]);
        col++;
        if (col >= 16) {
            col = 0;
            row++;
            if (row >= 8) {
                row = 0;
            }
        }
    }
}

void ClearString(char *str, int row, int col) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        LCDDrawChar(row,  col, str[i]);
        col++;
        if (col >= 16) {
            col = 0;
            row++;
            if (row >= 8) {
                row = 0;
            }
        }
    }
}
