/*	Author: brandont
 *  Partner(s) Name: Joseph DiCarlantonio jdica001@ucr.edu
 *	Lab Section:
 *	Assignment: Lab 11  Exercise 5
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

//#define CLOCK (PINC & 0x01)
//#define DATA (PORTC & 0x02)

int main(void) {
    //output of columns
    DDRC = 0xFF;
    PORTC = 0x00;
    
    //set port A as output columns
//    DDRA = 0xFF;
//    PORTA = 0x00;

    //set period timer
    TimerSet(1);
    //initialize timer
    TimerOn();
    unsigned char row = 0;
    unsigned char column = 0;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t k = 8;
    uint8_t state = 0;
    //initialize shift register
    shiftInit();
    shift2Init();
    while (1)
    {
        if(state == 0)
        {
            row = 7;
            column = 7;
            shift2Write(0xFF & (~(1 << row)) );
            shiftWrite(0x00 | (1 << column));
            state = 1;
        }
        else if(state == 1)
        {
            shift2Write(0xFF);
            shiftWrite(0x00);
            state = 2;
        }
        else if(state == 2)
        {
            row = 0;
            column = 0;
            shift2Write(0xFF & (~(1 << row)) );
            shiftWrite(0x00 | (1 << column));
            state = 3;
        }
        else
        {
            shift2Write(0xFF);
            shiftWrite(0x00);
            state = 0;
        }
//        shift2Write(0xFF & (~(1 << row)) );
//        shiftWrite(0x00 | (1 << column));
        while(!TimerFlag);
        TimerFlag = 0;
    }
}

