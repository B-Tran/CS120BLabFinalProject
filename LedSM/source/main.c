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
uint8_t Ballx;
uint8_t Bally;
uint8_t BallLost;

#include "../header/BallBounce.h"
#include "../header/Player1.h"
#include "../header/DisplayMatrix.h"
int main(void) {
    //randomize direction
    srand(time(0));
    //init buttons
    DDRA = 0x00;
    PORTA = 0xFF;
    //set period timer
    TimerSet(1);
    //initialize timer
    TimerOn();
    //initialize shift register
    shiftInit();
    shift2Init();
    DisplayState = Display_Start;
    Player1State = Player1_Start;
    BallState = Ball_Start;
    const uint8_t player1Period = 50;
    uint8_t player1ElaspsedTime = 50;
    const uint16_t BallPeriod = 500;
    uint16_t BallElaspsedTime = 500;
    while (1)
    {
        //write to specific leds
        DisplayArray[0][0] = 1;
        DisplayArray[1][0] = 1;
        DisplayArray[2][0] = 1;
        DisplayArray[3][0] = 1;
        DisplayArray[4][0] = 1;
        DisplayArray[5][0] = 1;
        DisplayArray[6][0] = 1;
        DisplayArray[7][0] = 1;
//        DisplayArray[][3] = 1;
        if(player1ElaspsedTime >= player1Period)
        {
            Player1SM();
            player1ElaspsedTime = 0;
        }
        else
        {
            player1ElaspsedTime++;
        }
        if(BallElaspsedTime >= BallPeriod)
        {
            BallSM();
            BallElaspsedTime = 0;
        }
        else
        {
            BallElaspsedTime++;
        }
        DisplaySM();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}

