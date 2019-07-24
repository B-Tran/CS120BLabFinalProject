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

//#include "../header/scheduler.h"
//#include "../header/mtimer.h"
//#include "../header/keypad.h"
//#include "../header/io.h"
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
uint8_t AIMode;
uint8_t AIButtons;
uint8_t AICenter;
const uint8_t AITop = 1;
const uint8_t AIBottom = MATRIX_SIZE - 2;
uint8_t begin;
uint8_t reset;

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

//AI BEGIN
enum AIStates {AI_Start, AI_Init, AI_Wait, AI_Move, AI_Stop} AIState;

void AISM()
{
    //transitions
    switch(AIState)
    {
        case AI_Start:
            AIState = AI_Init;
            break;
        case AI_Init:
            AIState = AI_Wait;
            break;
        case AI_Wait:
//            PORTC = 0x03;
            if(reset)
            {
                AIState = AI_Init;
            }
            else if(AIMode == 0x01 && begin)
            {
                AIState = AI_Move;
            }
            else
            {
                AIState = AI_Wait;
            }
            break;
        case AI_Move:
            //PORTC = 0x01;
            if(!BallLost)
            {
                AIState = AI_Move;
            }
            else
            {
                AIState = AI_Stop;
            }
            break;
        case AI_Stop:
            //PORTC = 0x02;
            AIState = AI_Init;
            break;
        default:
            AIState = AI_Start;
            break;
    }
    //actions
    switch(AIState)
    {
        case AI_Start:
            break;
        case AI_Init:
            AIButtons = 0x00;
            break;
        case AI_Move:
            if(AICenter > Bally && AICenter > AITop)
            {
                AIButtons = 0x01;
            }
            else if(AICenter < Bally && AICenter < AIBottom)
            {
                AIButtons = 0x02;
            }
            else
            {
                AIButtons = 0x00;
            }
            break;
        case AI_Stop:
            AIButtons = 0x00;
            break;
        default:
            break;
    }
}
//AI END

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
    //set period timer
    TimerSet(1);
    //initialize timer
    TimerOn();
    //initialize shift register
    shiftInit();
    shift2Init();
    DisplayState = Display_Start;
    GameState = Game_Start;
    AIState = AI_Start;
    Player1State = Player1_Start;
    BallState = Ball_Start;
    const uint16_t GamePeriod = 100;
    uint8_t GameElaspsedTime = 100;
    const uint16_t AIPeriod = 100;
    uint8_t AIElaspsedTime = 100;
    const uint8_t player1Period = 50;
    uint8_t player1ElaspsedTime = 50;
    const uint8_t player2Period = 50;
    uint8_t player2ElaspsedTime = 50;
    const uint16_t BallPeriod = 50;
    uint16_t BallElaspsedTime = 50;
    AIMode = 0x01;
    BallLost = 0x00;
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

