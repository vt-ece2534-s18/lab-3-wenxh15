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

 enum {Game, Play, History} S;//let the "->" point to the correct option//states of "->" of main menu option


//typedef enum {PlayGame, HowToPlay, ScoreHistory} menu_t;
//typedef enum {ContinueGame, MainMenu} pause_t;

bool Ingame = false;
bool ArrowMove = true;
bool pause = false;
bool GO;
bool next = false;
bool newgame = true;
bool hit1,hit2,hit3;


int16_t prevCarX = 15;
int16_t curCarX = 15;
int16_t prevObs1 = 0;
int16_t curObs1 = 0;
int16_t prevObs2 = 0;
int16_t curObs2 = 0;
int16_t prevObs3 = 0;
int16_t curObs3 = 0;
int16_t prevObs4 = 0;
int16_t curObs4 = 0;
int16_t obs0 = 15;
int16_t obs1 = 15;
int16_t obs2 = 15;
int16_t bonus;
int16_t bonusX = 15;
int16_t prevBonus = -1;
int16_t curBonus = -1;
int16_t acc = 1;


unsigned int vx, vy;
unsigned score, life;



int16_t randObsX()
{
    int place = rand()%3;
    if(place == 0)
    {
        return 15;
    }
    else if(place == 1)
    {
        return 31;
    }
    else if (place == 2)
    {
        return 47;
    }
   return -1;
}


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
    PrintString("LIFE", 6, 8);

    LCDDrawLine();
    LCDDrawCar(prevCarX, curCarX);
}

void DrawGameOverScreen()
{
    static OneShotSWTimer_t OST;
    bool swTimerExpired;
    InitOneShotSWTimer(&OST,TIMER32_1_BASE,OPENING_WAIT);
    LCDClearDisplay(MY_BLACK);
    PrintString("GAME OVER", 0,5);
    PrintString("1st",2,0);
    PrintString("2nd",4,0);
    PrintString("3rd",6,0);
    StartOneShotSWTimer(&OST);
    swTimerExpired = OneShotSWTimerExpired(&OST);
    while (!swTimerExpired)
    {
        swTimerExpired = OneShotSWTimerExpired(&OST);
    }
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
    typedef enum {ContinueGame, MainMenu} pause_t;
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
              newgame = false;
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
{
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
    typedef enum {PlayGame, HowToPlay, ScoreHistory} menu_t; //states of what process going
    static menu_t menuOption = PlayGame;
    switch(menuOption){
    case PlayGame:
        getSampleJoyStick(&vx, &vy);
        if (Booster_Joystick_Pushed())
          {
              DrawGameScreen();
              Ingame = true;
              GO = false;
              score = 0;
              ArrowMove = false;
              life = 3;
              bonus = 0;
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

void MoveObs()
{
    if(curObs1 == -1)
    {
        if(curObs3 == -1 || curObs3 > 50)
        {
            obs0 = randObsX();
            curObs1 = 0;
            prevObs1 = 0;
            hit1 = true;
        }
    }
    else if(curObs1 < 128 && curObs1 > -1)
    {
        curObs1 += acc;
        LCDDrawObs(prevObs1, curObs1, obs0);
        prevObs1 = curObs1;
    }
    else if(curObs1 >= 128)
    {
        curObs1 = -1;
        bonus--;
        score++;
    }

    if(curObs2 == -1)
    {
        if(curObs1 > 50)
        {
            obs1 = randObsX();
            curObs2 = 0;
            prevObs2 = 0;
            hit2 = true;
        }
    }
    else if(curObs2 < 128 && curObs2 > -1)
    {
        curObs2 += acc;
        LCDDrawObs(prevObs2, curObs2, obs1);
        prevObs2 = curObs2;
    }
    else if(curObs2 >= 128)
    {
        curObs2 = -1;
        bonus--;
        score++;
    }

    if(curObs3 == -1)
        {
            if(curObs2 > 50)
            {
                obs2 = randObsX();
                curObs3 = 0;
                prevObs3 = 0;
                hit3 = true;
            }
        }
        else if(curObs3 < 128 && curObs3 > -1)
        {
            curObs3 += acc;
            LCDDrawObs(prevObs3, curObs3, obs2);
            prevObs3 = curObs3;
        }
        else if(curObs3 >= 128)
        {
            curObs3 = -1;
            bonus--;
            score++;
        }

}


void MoveCar()
{
    getSampleJoyStick(&vx, &vy);
    if (vx > UP_THRESHOLD)
    {
        curCarX +=acc;
        if(prevCarX < 47)
        {
            LCDDrawCar(prevCarX, curCarX);
            prevCarX = curCarX;
        }
        else
        {
            curCarX = 47;
            LCDDrawCar(prevCarX, curCarX);
            prevCarX = curCarX;
        }
    }
    if (vx < DOWN_THRESHOLD)
    {
        curCarX -= acc;
        if(prevCarX > 15)
        {
            LCDDrawCar(prevCarX, curCarX);
            prevCarX = curCarX;
        }
        else
        {
            curCarX=15;
            LCDDrawCar(prevCarX, curCarX);
            prevCarX = curCarX;
        }
    }
}

void Evaluate()
{
    if(curObs1 >= 97 && curObs1 <= 127)
    {
        if(obs0 > curCarX + 15 || curCarX > obs0 + 15)
        {

        }
        else
        {
            if(hit1)
            {
                life--;
                hit1 = false;
            }
        }
    }
    if(curObs2 >= 97 && curObs2 <= 127)
        {
            if(obs1 > curCarX + 15 || curCarX > obs1 + 15)
            {

            }
            else
            {
                if(hit2)
                {
                    life--;
                    hit2 = false;
                }
            }
        }
    if(curObs3 >= 97 && curObs3 <= 127)
        {
            if(obs2 > curCarX + 15 || curCarX > obs2 + 15)
            {

            }
            else
            {
                if(hit3)
                {
                    life--;
                    hit3 = false;
                }
            }
        }
    if(curBonus >=97 && curBonus <=127)
    {
        if(bonusX > curCarX + 15 || curCarX > bonusX + 15)
        {

        }
        else
        {
            curBonus = 128;
            LCDDrawBonus(prevBonus, curBonus, bonusX);
            curBonus = -1;
            life++;
        }
    }
    if(life <= 0)
    {
        GO = true;
    }
}

void MoveBonus()
{
    if(curBonus == -1)
    {
        if(bonus <= 0)
        {
            if((curObs1 >15 || curObs1 == -1) && (curObs2 >15 || curObs2 == -1) && (curObs3 >15 || curObs3 == -1))
            {
                bonusX = randObsX();
                curBonus = 0;
                prevBonus = 0;
                bonus = rand()%5;
            }
        }
    }
    else if(curBonus < 128 && curBonus > -1)
    {
        curBonus += acc;
        LCDDrawBonus(prevBonus, curBonus, bonusX);
        prevBonus = curBonus;
    }
    else if(curBonus >= 128)
    {
        curBonus = -1;
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

    static OneShotSWTimer_t CAR;
    bool swTimerExpired;
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
                InitOneShotSWTimer(&CAR, TIMER32_1_BASE, CAR_WAIT);
                StartOneShotSWTimer(&CAR);
                swTimerExpired = OneShotSWTimerExpired(&CAR);
                while (!swTimerExpired)
                    {
                        swTimerExpired = OneShotSWTimerExpired(&CAR);
                    }
                MoveCar();
                MoveObs();
                MoveBonus();
                Evaluate();
                char buf = ((score/10)%10)+'0';
                PrintString(&buf, 1, 8);
                buf = (score%10)+'0';
                PrintString(&buf, 1, 9);
                buf = (life%10)+'0';
                PrintString(&buf, 7, 8);
                getSampleJoyStick(&vx, &vy);
                if (vy > UP_THRESHOLD)
                {
                    acc ++;
                }
                else if (vy < DOWN_THRESHOLD)
                {
                    if(acc > 1)
                    {
                        acc--;
                    }
                }
            }
            if(GO)
            {
                DrawGameOverScreen();
                DrawMenuScreen();
                PrintMenuOption(Game);
                ArrowMove = true;
                Ingame = false;
            }
        }
        else
         {
            MainMenuFSM();
            curObs1 = -1;
            curObs2 = -1;
            curObs3 = -1;
            prevCarX = 15;
            curCarX = 15;
            acc = 1;
         }
    }

}
