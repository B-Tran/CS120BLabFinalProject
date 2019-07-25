/*	Author: brandont
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab 12  Exercise Final Project
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <time.h>
#include <stdlib.h>
#include <time.h>

//#include "../header/scheduler.h"
//#include "../header/mtimer.h"
//#include "../header/keypad.h"
#include "../header/io.h"
#include "../header/timer.h"
#include "../header/shiftRegister1.h"
#include "../header/shiftRegister2.h"
//#include "../header/pwm.h"
#ifdef _SIMULATE_
//#include "simAVRHeader.h"
#endif

#define MATRIX_SIZE 8
uint8_t DisplayArray[MATRIX_SIZE][MATRIX_SIZE];
#define GET_BUTTONS (~PINA & 0x03)
#define GET_BUTTONS2 ((~PINA & 0x0C) >> 2)

uint8_t Ballx;
uint8_t Bally;
uint8_t BallLost;
uint8_t BallPoint;
uint8_t AIMode;
uint8_t AIButtons;
uint8_t AICenter;
const uint8_t AITop = 1;
const uint8_t AIBottom = MATRIX_SIZE - 2;
uint8_t begin;
uint8_t reset;
uint8_t randNum;

//SCORE BEGIN
enum ScoreStates {Score_Start, Score_Init, Score_Wait, Score_Point,
    Score_Check ,Score_Done} 
            ScoreState;
void PrintWinMsg(char player)
{
    char message3[14] = "PlayerX Wins.";
    message3[6] = player;
    LCD_ClearScreen();
    for(uint8_t i = 0; i < 13; ++i)
    {
        LCD_Cursor(i+1);
        LCD_WriteData(message3[i]);
    }
}

void PrintPointMsg(uint8_t player1, uint8_t player2)
{
    char message1[12] = "Player1 : ";
    char message2[12] = "Player2 : ";
    message1[10] = (player1 + '0');
    message2[10] = (player2 + '0');
    for(uint8_t i = 0; i < 11; ++i)
    {
        LCD_Cursor(i+1);
        LCD_WriteData(message1[i]);
        LCD_Cursor(i+17);
        LCD_WriteData(message2[i]);
    }

}
void ScoreSM()
{
    static uint8_t player1Pnt;
    static uint8_t player2Pnt;
    static const uint8_t maxPnt = 3;
    //transitions
    switch(ScoreState)
    {
        case Score_Start:
            ScoreState = Score_Init;
            break;
        case Score_Init:
            ScoreState = Score_Wait;
            break;
        case Score_Wait:
            if(reset)
            {
                ScoreState = Score_Init;
            }
            else if(begin && !reset)
            {
                ScoreState = Score_Point;
                LCD_ClearScreen();
                PrintPointMsg(player1Pnt, player2Pnt);
            }
            else
            {
                ScoreState = Score_Wait;
            }
            break;
        case Score_Point:
            if(reset)
            {
                ScoreState = Score_Init;
            }
            else if(!BallPoint && !reset)
            {
                ScoreState = Score_Point;
            }
            else
            {
                if(Ballx >= (MATRIX_SIZE - 2))
                {
                    player2Pnt++;
//                    PrintPointMsg(player1Pnt, player2Pnt);
                }
                else
                {
                    player1Pnt++;
//                    PrintPointMsg(player1Pnt, player2Pnt);
                }
                ScoreState = Score_Check;
            }
            break;
        case Score_Check:
            if(reset)
            {
                ScoreState = Score_Init;
            }
            else if((player1Pnt < maxPnt) && (player2Pnt < maxPnt))
            {
                PrintPointMsg(player1Pnt, player2Pnt);
                ScoreState = Score_Wait;
            }
            else
            {
                ScoreState = Score_Done;
            }
            break;
        case Score_Done:
            ScoreState = Score_Wait;
        default:
            break;
    }
    //actions
    switch(ScoreState)
    {
        case Score_Start:
            break;
        case Score_Init:
            player1Pnt = 0;
            player2Pnt = 0;
            LCD_ClearScreen();
            PrintPointMsg(player1Pnt, player2Pnt);
            break;
        case Score_Wait:
            BallPoint = 0;
            break;
        case Score_Point:
            break;
        case Score_Done:
            if(player1Pnt >= maxPnt)
            {
                PrintWinMsg('1');
            }
            else
            {
                PrintWinMsg('2');
            }
            player1Pnt = 0;
            player2Pnt = 0;
            break;
        case Score_Check:
            break;
        default:
            break;
    }
}
//SCORE END

//GAME BEGIN
enum GameStates {Game_Start, Game_Init, Game_Wait, Game_Begin,
                        Game_Reset, Game_Reset_Done} GameState;

void GameSM()
{
    uint8_t buttons = GET_BUTTONS2;
    //transitions
    switch(GameState)
    {
        case Game_Start:
            GameState = Game_Init;
            break;
        case Game_Init:
            GameState = Game_Wait;
            break;
        case Game_Wait:
//            PORTC = 0x01;
            if(buttons == 0x01 && !begin)
            {
                GameState = Game_Begin;
            }
            else if(buttons == 0x02)
            {
                GameState = Game_Reset;
            }
            else
            {
                GameState = Game_Wait;
            }
            break;
        case Game_Begin:
            if(buttons == 0x00)
            {
                GameState = Game_Wait;
                begin = 0x01;
                reset = 0x00;
            }
            else
            {
                GameState = Game_Begin;
            }
            break;
        case Game_Reset:
            if(buttons == 0x00)
            {
                GameState = Game_Reset_Done;
                begin = 0x00;
                reset = 0x01;
            }
            else
            {
                GameState = Game_Reset;
            }
            break;
        case Game_Reset_Done:
            GameState = Game_Init;
            break;
        default:
            GameState = Game_Start;
            break;
    }
    //actions
    switch(GameState)
    {
        case Game_Start:
            break;
        case Game_Init:
            begin = 0x00;
            reset = 0x00;
            break;
        case Game_Wait:
            break;
        case Game_Reset:
            break;
        case Game_Reset_Done:
            break;
        default:
            break;
    }
}
//GAME END


#include "../header/AI.h"
#include "../header/BallBounce.h"
#include "../header/Player1.h"
#include "../header/Player2.h"
#include "../header/DisplayMatrix.h"
int main(void) {
    //randomize direction
    srand(time(0));
    //init buttons
    DDRA = 0x00;
    PORTA = 0xFF;
    DDRC = 0xFF;
    PORTC = 0x00;
    DDRD = 0xFF;
    PORTD = 0x00;
    //set period timer
    TimerSet(1);
    //initialize timer
    TimerOn();
    //initialize shift register
    shiftInit();
    shift2Init();
    //initialize lcd
    LCD_init();
    //tasks
    DisplayState = Display_Start;
    ScoreState = Score_Start;
    GameState = Game_Start;
    AIState = AI_Start;
    Player1State = Player1_Start;
    BallState = Ball_Start;
    const uint16_t ScorePeriod = 100;
    uint8_t ScoreElaspsedTime = 100;
    const uint16_t GamePeriod = 100;
    uint8_t GameElaspsedTime = 100;
    const uint16_t AIPeriod = 80;
    uint8_t AIElaspsedTime = 80;
    const uint8_t player1Period = 40;
    uint8_t player1ElaspsedTime = 40;
    const uint8_t player2Period = 40;
    uint8_t player2ElaspsedTime = 40;
    const uint16_t BallPeriod = 50;
    uint16_t BallElaspsedTime = 50;
    AIMode = 0x01;
    BallLost = 0x00;
    randNum = 0;
    while (1)
    {
        //write to specific leds
//        DisplayArray[0][0] = 1;
//        DisplayArray[1][0] = 1;
//        DisplayArray[2][0] = 1;
//        DisplayArray[3][0] = 1;
//        DisplayArray[4][0] = 1;
//        DisplayArray[5][0] = 1;
//        DisplayArray[6][0] = 1;
//        DisplayArray[7][0] = 1;
//        DisplayArray[][3] = 1;
        if(randNum < 254)
        {
            randNum++;
        }
        else
        {
            randNum = 0;
        }
        if(ScoreElaspsedTime >= ScorePeriod)
        {
            ScoreSM();
            ScoreElaspsedTime = 0;
        }
        else
        {
            ScoreElaspsedTime++;
        }
        if(GameElaspsedTime >= GamePeriod)
        {
            GameSM();
            GameElaspsedTime = 0;
        }
        else
        {
            GameElaspsedTime++;
        }
        if(AIElaspsedTime >= AIPeriod)
        {
            AISM();
            AIElaspsedTime = 0;
        }
        else
        {
            AIElaspsedTime++;
        }
        if(player1ElaspsedTime >= player1Period)
        {
            Player1SM();
            player1ElaspsedTime = 0;
        }
        else
        {
            player1ElaspsedTime++;
        }
        if(player2ElaspsedTime >= player2Period)
        {
            Player2SM();
            player2ElaspsedTime = 0;
        }
        else
        {
            player2ElaspsedTime++;
        }
        if(BallElaspsedTime >= BallPeriod)
        {
//            PORTC = 0x01;
            BallSM();
            BallElaspsedTime = 0;
        }
        else
        {
//            PORTC = 0x02;
            BallElaspsedTime++;
        }
        DisplaySM();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}

