/*
 * You can add more of your own developed *.h files here.
 * However, you are not allowed to include driverlib, grlib, stdio or any other library in this file.
 */

#include <LED_HAL.h>
#include <Buttons_HAL.h>
#include <Timer_HAL.h>
#include <Display_HAL.h>
#include <ADC_HAL.h>

#define OPENING_WAIT 3000 // 3 seconds or 3000 ms
#define CAR_WAIT 50
#define OBS_WAIT 50
#define UP_THRESHOLD  0x3000
#define DOWN_THRESHOLD 0x1000
#define MIDDLE_MAX_THRESHOLD 0x1500
#define MIDDLE_MIN_THRESHOLD 0x2500

enum {Game, Play, History} S; //states of "->" of main menu option

typedef enum {PlayGame, HowToPlay, ScoreHistory} menu_t;
typedef enum {ContinueGame, MainMenu} pause_t;

bool Ingame = false;
bool ArrowMove = true;
bool pause = false;
bool GO;

int16_t prevCarX = 16;
int16_t curCarX = 16;
int16_t prevObs1;
int16_t curObs1;
int16_t prevObs2;
int16_t curObs2;
int16_t prevObs3;
int16_t curObs3;

unsigned int vx, vy;

void DrawOpeningScreen()
{
    LCDClearDisplay(MY_BLACK);
    PrintString("Car GAME", 1, 4);
    PrintString("by", 3, 7);
    PrintString("Wenxuan He", 5, 3);
}

void DrawMenuScreen()
{
    LCDClearDisplay(MY_BLACK);
    PrintString("GAME MENU",0,4);
    PrintString("Play Game", 2, 3);
    PrintString("How to Play", 4, 3);
    PrintString("Score History", 6, 3);
    PrintString("->",2,1);
}

void DrawInstructionScreen()
{
    LCDClearDisplay(MY_BLACK);
    PrintString("INSTRUCTIONS", 0,2);
    PrintString("this is the instruction blablablablablablablablab",2,0);
    PrintString("BTN1-MENU",7,7);
}

void JoystickBacktoMiddle()
{//make joystick back to middle
    getSampleJoyStick(&vx, &vy);
    while (vy < MIDDLE_MAX_THRESHOLD || vy > MIDDLE_MIN_THRESHOLD)
        {
        getSampleJoyStick(&vx, &vy);
        }
}

void DrawScoreScreen()
{
    LCDClearDisplay(MY_BLACK);
    PrintString("SCORES", 0,5);
    PrintString("1st",2,0);
    PrintString("2nd",4,0);
    PrintString("3rd",6,0);
    PrintString("BTN1-MENU",7,7);
}

void DrawGameScreen()
{
    LCDClearDisplay(MY_BLACK);
    PrintString("SCORE", 0, 8);
    PrintString("HIGH", 3, 8);
    PrintString("SCORE", 4, 8);
    LCDDrawLine();
    LCDDrawCar(prevCarX, curCarX);
}

void DrawGameOverScreen()
{
    LCDClearDisplay(MY_BLACK);
    PrintString("GAME OVER", 0,5);
    PrintString("1st",2,0);
    PrintString("2nd",4,0);
    PrintString("3rd",6,0);
    PrintString("BTN1-MENU",7,7);
}
void DrawPauseScreen()
{
    LCDClearDisplay(MY_BLACK);
    PrintString("GAME PAUSED", 0,3);
    PrintString("Continue", 2, 3);
    PrintString("Menu", 4, 3);
    PrintString("->",2,1);
}

void PauseScreenFSM()
{
    //typedef enum {ContinueGame, MainMenu} pause_t;
    static pause_t s = ContinueGame;
    switch (s)
    {
    case ContinueGame:
        getSampleJoyStick(&vx, &vy);
        if (Booster_Joystick_Pushed())
          {
              DrawGameScreen();
              pause = false;
          }
                if (vy > UP_THRESHOLD || vy < DOWN_THRESHOLD)
                    {
                        JoystickBacktoMiddle();
                        ClearString("->",2,1);
                        PrintString("->",4,1);
                        s = MainMenu;
                    }
        break;
    case MainMenu:
        getSampleJoyStick(&vx, &vy);
        if (Booster_Joystick_Pushed())
          {
              DrawMenuScreen();
              pause = false;
              Ingame = false;
              s = ContinueGame;
          }
                 if (vy > UP_THRESHOLD || vy < DOWN_THRESHOLD)
                {
                    JoystickBacktoMiddle();
                    ClearString("->",4,1);
                    PrintString("->",2,1);
                    s = ContinueGame;
                }
        break;
    }
}

void PrintMenuOption(S)
{//let the "->" point to the correct option
    switch(S){
    case Play:
        ClearString("->",2,1);
        ClearString("->",6,1);
        PrintString("->",4,1);
    break;
    case History:
        ClearString("->",2,1);
        ClearString("->",4,1);
        PrintString("->",6,1);
    break;
    case Game:
        ClearString("->",4,1);
        ClearString("->",6,1);
        PrintString("->",2,1);
    break;
    }
}

void MainMenuFSM()
{// choose which screen will showing up next
    //typedef enum {PlayGame, HowToPlay, ScoreHistory} menu_t; //states of what process going
    static menu_t menuOption = PlayGame;
    switch(menuOption){
    case PlayGame:
        getSampleJoyStick(&vx, &vy);
        if (Booster_Joystick_Pushed())
          {
              DrawGameScreen();
              Ingame = true;
              GO = false;
          }
        if (ArrowMove)
        {
            if (vy > UP_THRESHOLD)
            {
                JoystickBacktoMiddle();
                PrintMenuOption(History);
                menuOption = ScoreHistory;
            }
            if (vy < DOWN_THRESHOLD)
            {
                JoystickBacktoMiddle();
                PrintMenuOption(Play);
                menuOption = HowToPlay;
            }
        }
    break;
    case HowToPlay:
        getSampleJoyStick(&vx, &vy);
        if (Booster_Joystick_Pushed())
         {
             DrawInstructionScreen();
             ArrowMove = false;
         }
        if(ArrowMove)
        {
                    if (vy > UP_THRESHOLD)
                    {
                        JoystickBacktoMiddle();
                        PrintMenuOption(Game);
                        menuOption = PlayGame;
                    }
                    else if (vy < DOWN_THRESHOLD)
                    {
                        JoystickBacktoMiddle();
                        PrintMenuOption(History);
                        menuOption = ScoreHistory;
                    }
        }
        else
        {
            if (Booster_Top_Button_Pushed())
                 {
                        DrawMenuScreen();
                        PrintMenuOption(Play);
                        ArrowMove = true;
                 }
        }
    break;
    case ScoreHistory:
        getSampleJoyStick(&vx, &vy);
        if (Booster_Joystick_Pushed())
        {
            DrawScoreScreen();
            ArrowMove = false;
        }
        if (ArrowMove)
        {
            if (vy > UP_THRESHOLD)
            {
                JoystickBacktoMiddle();
                PrintMenuOption(Play);
                menuOption = HowToPlay;
            }
            else if (vy < DOWN_THRESHOLD)
            {
                JoystickBacktoMiddle();
                PrintMenuOption(Game);
                menuOption = PlayGame;
            }
        }
        else
        {
            if (Booster_Top_Button_Pushed())
                 {
                DrawMenuScreen();
                PrintMenuOption(History);
                ArrowMove = true;
                 }
        }
    break;
    }
}

void ScreensFSM()
{
    static enum states {INCEPTION, OPENING, MENU} state = INCEPTION;
    static OneShotSWTimer_t OST;//, CAROST, OBSOST;
    //static bool newGame;
    // Set the default outputs
    bool drawOpeningScreen = false;
    bool drawMenuScreen = false;
    bool startSWTimer = false;
    //bool startCARTimer = false;
    //bool startOBSTimer = false;

    // Inputs of the FSM
    bool swTimerExpired;

    /*InitOneShotSWTimer(&OST,
                       TIMER32_1_BASE,
                       OPENING_WAIT);
    InitOneShotCARTimer(&CAROST,
                       TIMER32_1_BASE,
                       CAR_WAIT);
    InitOneShotOBSTimer(&OBSOST,
                       TIMER32_1_BASE,
                       OBS_WAIT);
*/

    switch (state)
    {
    case INCEPTION:
        //StartOneShotSWTimer(&OST);

        // State transition
        state = OPENING;

        // The output(s) that are affected in this transition
        drawOpeningScreen = true;
        startSWTimer = true;
        break;

    case OPENING:
        swTimerExpired = OneShotSWTimerExpired(&OST);
        if (swTimerExpired)
        {
            // State transition
            state = MENU;

            // The output(s) that are affected in this transition
            drawMenuScreen = true;
        }
        break;

    case MENU:

        break;

    } // End of switch-case

    // Implement actions based on the outputs of the FSM
    if (startSWTimer)
    {
        InitOneShotSWTimer(&OST, TIMER32_1_BASE, OPENING_WAIT);
        StartOneShotSWTimer(&OST);
    }

    if (drawOpeningScreen)
       DrawOpeningScreen();

    if (drawMenuScreen)
       DrawMenuScreen();
}

void MoveCar()
{
    getSampleJoyStick(&vx, &vy);
    if (vx > UP_THRESHOLD)
    {
        if(prevCarX < 48)
        {
            curCarX++;
            LCDDrawCar(prevCarX, curCarX);
            prevCarX = curCarX;
        }
    }
    else if (vx < DOWN_THRESHOLD)
    {
        if(prevCarX > 16)
        {
            curCarX--;
            LCDDrawCar(prevCarX, curCarX);
            prevCarX = curCarX;
        }
    }
}


int main(void) {

    WDT_A_hold(WDT_A_BASE);

    BSP_Clock_InitFastest();
    initADC_Multi();
    InitGraphics();
    initJoyStick();
    InitHWTimers();
    InitButtons();
    InitLEDs();

    StartOneShotHWTimer();
    startADC();
    static OneShotSWTimer_t A;
    bool swTimerExpired;
    InitOneShotSWTimer(&A, TIMER32_1_BASE, CAR_WAIT);

    while (1)
    {
        // Do not delete this statement. We will use this function to check if your program does not block on anything.
      //  if (Booster_Bottom_Button_Pushed())
           // Toggle_Launchpad_Right_Red_LED();


        ScreensFSM();
       // getSampleJoyStick(&vx, &vy);
        if (Ingame)
        {
            if (Booster_Top_Button_Pushed())
                 {
                    DrawPauseScreen();
                    pause = true;
                 }
            if(pause)
            {

                PauseScreenFSM();
                //if (Booster_Bottom_Button_Pushed())
                                           //Toggle_Launchpad_Right_Red_LED();
            }
            else
            {
                    swTimerExpired = OneShotSWTimerExpired(&A);
                    if(swTimerExpired)
                    {
                        MoveCar();
                        //MoveObs();
                        //Evaluate();
                    }


            }
            if(GO)
            {
                //DrawGameOver();
                Ingame = false;
            }
        }
        else
         {
            MainMenuFSM();
           // if (Booster_Bottom_Button_Pushed())
                        //      Toggle_Launchpad_Right_Red_LED();

         }
        if(Ingame)
        {
            if(!pause)
            {
                StartOneShotSWTimer(&A);
            }
        }
    }
}



