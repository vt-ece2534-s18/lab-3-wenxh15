/*
 * You can add more of your own developed *.h files here.
 * However, you are not allowed to include driverlib, grlib, stdio or any other library in this file.
 */

#include <LED_HAL.h>
#include <Buttons_HAL.h>
#include <Timer_HAL.h>
#include <Display_HAL.h>
#include <ADC_HAL.h>
#include <stdlib.h>

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
bool next = false;


int16_t prevCarX = 15;
int16_t curCarX = 15;
int16_t prevObs1 = 0;
int16_t curObs1 = 0;
int16_t prevObs2 = 0;
int16_t curObs2 = 0;
int16_t prevObs3 = 0;
int16_t curObs3 = 0;
int16_t ObsX = 15;
int16_t prevObsY = 0;
int16_t curObsY = 0;

unsigned int vx, vy;
int Count;


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
    LCDDrawObs(ObsX, prevObsY, curObsY);
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
              Ingame = false;
              pause = false;
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
/*
void MoveObs()
{
    int numberObs = rand()%3;
    static OneShotSWTimer_t OBS;
    bool swTimerExpired;
    InitOneShotSWTimer(&OBS, TIMER32_1_BASE, OBS_WAIT);
    if (numberObs == 0)
    {
        if (curObs1 < 127)
        {
            StartOneShotSWTimer(&OBS);
            swTimerExpired = OneShotSWTimerExpired(&OBS);
            while (!swTimerExpired)
            {
                swTimerExpired = OneShotSWTimerExpired(&OBS);
            }
            curObs1++;
            LCDDrawObs1(prevObs1, curObs1);
            prevObs1 = curObs1;
            if (curObs1 < 50)
            {
                next = false;
            }
        }
    }
    else if(numberObs == 1)
    {
        if (curObs2 < 127)
        {
            StartOneShotSWTimer(&OBS);
            swTimerExpired = OneShotSWTimerExpired(&OBS);
            while (!swTimerExpired)
            {
                swTimerExpired = OneShotSWTimerExpired(&OBS);
            }
            curObs2++;
            LCDDrawObs2(prevObs2, curObs2);
            prevObs2 = curObs2;
            if (curObs2 < 50)
            {
                next = false;
            }
        }
    }
    else if(numberObs == 2)
    {
        if (curObs1 < 127)
        {
            StartOneShotSWTimer(&OBS);
            swTimerExpired = OneShotSWTimerExpired(&OBS);
            while (!swTimerExpired)
            {
                swTimerExpired = OneShotSWTimerExpired(&OBS);
            }
            curObs3++;
            LCDDrawObs3(prevObs3, curObs3);
            prevObs3 = curObs3;
            if (curObs3 < 50)
            {
                next = false;
            }
        }
    }
}
*/
void MoveObs()
{
   // int numberObs = rand()%3;
    static OneShotSWTimer_t OBS;
    bool swTimerExpired;
    InitOneShotSWTimer(&OBS, TIMER32_1_BASE, OBS_WAIT);
  //  if (numberObs == 0)
   // {
      //  ObsX = 15;
  //  }
  //  else if (numberObs == 1)
  //  {
   //     ObsX = 31;
   // }
  //  else if (numberObs == 2)
  //  {
  //      ObsX = 47;
   // }
             if (curObsY < 129)
             {
                 StartOneShotSWTimer(&OBS);
                 swTimerExpired = OneShotSWTimerExpired(&OBS);
                 while (!swTimerExpired)
                 {
                     swTimerExpired = OneShotSWTimerExpired(&OBS);
                 }
                 curObsY++;
                 LCDDrawObs(ObsX, prevObsY, curObsY);
                 prevObsY = curObsY;
             }


    }

void MoveCar()
{
    static OneShotSWTimer_t CAR;
    bool swTimerExpired;
    InitOneShotSWTimer(&CAR, TIMER32_1_BASE, CAR_WAIT);
    StartOneShotSWTimer(&CAR);
    swTimerExpired = OneShotSWTimerExpired(&CAR);
    while (!swTimerExpired)
    {
        swTimerExpired = OneShotSWTimerExpired(&CAR);
    }
    getSampleJoyStick(&vx, &vy);
    if (vx > UP_THRESHOLD)
    {
        if(prevCarX < 47)
        {
            curCarX++;
            LCDDrawCar(prevCarX, curCarX);
            prevCarX = curCarX;
        }
    }
    if (vx < DOWN_THRESHOLD)
    {
        if(prevCarX > 15)
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

    startADC();

    while (1)
    {
        // Do not delete this statement. We will use this function to check if your program does not block on anything.
        if (Booster_Bottom_Button_Pushed())
            Toggle_Launchpad_Right_Red_LED();
        ScreensFSM();
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
            }
            else
            {
                MoveObs();
                MoveCar();




                //Evaluate();
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
         }
    }

}
