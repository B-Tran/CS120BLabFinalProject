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

#include "../header/scheduler.h"
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

//---------------SHARED-VARIABLES-------------
#define MATRIX_SIZE 8
uint8_t DisplayArray[MATRIX_SIZE][MATRIX_SIZE];
#define GET_BUTTONSP1 (~PINA & 0x03)
#define GET_BUTTONSSR ((~PINA & 0x0C) >> 2)
#define GET_AI ((~PINA & 0x10) >> 4)
#define GET_BUTTONSP2 ((~PINA & 0x60) >> 5)

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
uint8_t RoundDone;
//--------------------------------------------
#include "../header/AI.h"
#include "../header/BallBounce.h"
#include "../header/Player1.h"
#include "../header/Player2.h"
#include "../header/DisplayMatrix.h"
#include "../header/Game.h"
#include "../header/Score.h"
task AITask;
task BallTask;
task Player1Task;
task Player2Task;
task DisplayTask;
task GameTask;
task ScoreTask;
#define tasksPeriod 1

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
    TimerSet(tasksPeriod);
    //initialize timer
    TimerOn();
    //initialize shift register
    shiftInit();
    shift2Init();
    //initialize lcd
    LCD_init();
    //intitialize task
    task *tasks[] = {&ScoreTask, &GameTask, &AITask, &Player1Task,
                                &Player2Task, &BallTask,&DisplayTask};
    const uint8_t tasksSize = sizeof(tasks)/sizeof(tasks[0]);
    //tasks intitial state
    DisplayTask.state = Display_Start;
    ScoreTask.state = Score_Start;
    GameTask.state = Game_Start;
    AITask.state = AI_Start;
    Player1Task.state = Player1_Start;
    BallTask.state = Ball_Start;
    //tasks intitial period
    ScoreTask.period = 100;
    GameTask.period = 100;
    AITask.period = 80;
    Player1Task.period = 40;
    Player2Task.period = 40;
    BallTask.period = 50;
    DisplayTask.period = 1;
    //tasks intitial elapsed time
    ScoreTask.elapsedTime = 100;
    GameTask.elapsedTime = 100;
    AITask.elapsedTime = 80;
    Player1Task.elapsedTime = 40;
    Player2Task.elapsedTime = 40;
    BallTask.elapsedTime = 50;
    DisplayTask.elapsedTime = 1;
    //tasks state machine
    ScoreTask.TickFct = &ScoreSM;
    GameTask.TickFct = &GameSM;
    AITask.TickFct = &AISM;
    Player1Task.TickFct = &Player1SM;
    Player2Task.TickFct = &Player2SM;
    BallTask.TickFct = &BallSM;
    DisplayTask.TickFct = &DisplaySM;
    randNum = 0;
    while (1)
    {
        if(randNum < 254)
        {
            randNum++;
        }
        else
        {
            randNum = 0;
        }
        for(uint8_t i = 0; i< tasksSize; ++i)
        {
            //if task is ready to exexute
            if(tasks[i]->elapsedTime >= tasks[i]->period)
            {
                //execute tick function and update state
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                //reset elasped time
                tasks[i]->elapsedTime = 0;
            }
            //update elasped time
                tasks[i]->elapsedTime += tasksPeriod;
        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
}

