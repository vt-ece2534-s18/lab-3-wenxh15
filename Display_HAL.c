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
    Graphics_drawLineV(&g_sContext,15,0,128);
    Graphics_drawLineV(&g_sContext,64,0,128);

}

void LCDDrawCar(int16_t prevCarX, int16_t curCarX)
{
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_Rectangle R;
    R.xMin = prevCarX;
    R.xMax = prevCarX+15;
    R.yMin = 0;
    R.yMax = 15;
    Graphics_fillRectangle(&g_sContext, &R);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_YELLOW);
    R.xMin = curCarX;
    R.xMax = curCarX+15;
    R.yMin = 0;
    R.yMax = 15;
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
