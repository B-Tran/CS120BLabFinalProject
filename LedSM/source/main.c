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

enum DisplayStates {Display_Start, Display_Init, Display_Print} DisplayState;

void DisplaySM()
{
    static uint8_t row_cnt;
    static uint8_t col;
    static uint8_t print_flag;
    //transitions
    switch(DisplayState)
    {
        case Display_Start:
            DisplayState = Display_Init;
            break;
        case Display_Init:
            DisplayState = Display_Print;
            break;
        case Display_Print:
            DisplayState = Display_Print;
            break;
    }

    //actions
    switch(DisplayState)
    {
        case Display_Start:
            break;
        case Display_Init:
            for(uint8_t i =0; i < MATRIX_SIZE; ++i)
            {
                for(uint8_t j =0; j < MATRIX_SIZE; ++j)
                {
                    DisplayArray[i][j] = 0;
                }
            }
            row_cnt = 0;
            print_flag = 0;
            break;
        case Display_Print:
            col = 0x00;
            for(uint8_t i = 0; i < MATRIX_SIZE; ++i)
            {
                col |= (DisplayArray[row_cnt][i] << i);
            }
            if(print_flag == 0)
            {
                shift2Write(0xFF);
                shiftWrite(0x00);
                print_flag = 1;
                row_cnt++;
            }
            else
            {
                shift2Write(0xFF & (~(1 << row_cnt)) );
                shiftWrite(0x00 | (col));
                print_flag = 0;
            }
            if(row_cnt >= 8)
            {
                row_cnt = 0;
            }
            break;
    }
}

int main(void) {

    //set period timer
    TimerSet(1);
    //initialize timer
    TimerOn();
    uint8_t i = 0;
    uint8_t j = 0;
    uint16_t k = 0;
    uint8_t state = 0;
    //initialize shift register
    shiftInit();
    shift2Init();
    DisplayState = Display_Start;
    while (1)
    {
        //write to specific leds
        DisplayArray[0][0] = 1;
        DisplayArray[7][7] = 1;
        DisplayArray[5][5] = 1;
        DisplaySM();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}

