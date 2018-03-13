/*
 * You can add more of your own developed *.h files here.
 * However, you are not allowed to include driverlib, grlib, stdio or any other library in this file.
 */

#include <LED_HAL.h>
#include <Buttons_HAL.h>
#include <Timer_HAL.h>
#include <Display_HAL.h>

#define OPENING_WAIT 10000 // 10 seconds or 10000 ms

void DrawOpeningScreen()
{
    LCDClearDisplay(MY_BLACK);
    PrintString("YOUR GAME", 2, 2);
    PrintString("by", 3, 3);
    PrintString("YOUR NAME", 4, 2);
    PrintString("Wait 10 sec", 6, 1);
}

void DrawMenuScreen()
{
    LCDClearDisplay(MY_BLACK);
    PrintString("OPTION1", 2, 3);
    PrintString("OPTION2", 4, 3);
}

void ScreensFSM()
{
    static enum states {INCEPTION, OPENING, MENU} state = INCEPTION;
    static OneShotSWTimer_t OST;
    InitOneShotSWTimer(&OST,
                       TIMER32_1_BASE,
                       OPENING_WAIT);

    // Set the default outputs
    bool drawOpening = false;
    bool drawMenu = false;

    switch (state)
    {
    case INCEPTION:
        StartOneShotSWTimer(&OST);

        // State transition
        state = OPENING;

        // The output(s) that are affected in this transition
        drawOpening = true;
        break;

    case OPENING:
        if (OneShotSWTimerExpired(&OST))
        {
            // State transition
            state = MENU;

            // The output(s) that are affected in this transition
            drawMenu = true;
        }
        break;

    case MENU:
        break;

    } // End of switch-case

    // Implement actions based on the outputs of the FSM
    if (drawOpening)
            DrawOpeningScreen();

    if (drawMenu)
        DrawMenuScreen();

}

int main(void) {

    WDT_A_hold(WDT_A_BASE);

    BSP_Clock_InitFastest();
    InitGraphics();
    InitHWTimers();
    InitButtons();
    InitLEDs();

    while (1) {
        // Do not delete this statement. We will use this function to check if your program does not block on anything.
        if (Booster_Bottom_Button_Pushed())
            Toggle_Launchpad_Right_Red_LED();


        ScreensFSM();
    }

}
