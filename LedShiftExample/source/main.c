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
    TimerSet(250);
    //initialize timer
    TimerOn();
    unsigned char row = 0;
    unsigned char column = 0;
    //initialize shift register
    shiftInit();
    while (1)
    {
        if(row < 7)
        {
            if(column < 7)
            {
                column++;
            }
            else
            {
                row++;
                column = 0;
            }
        }
        else
        {
            if(column < 7)
            {
                column++;
            }
            else
            {
                row = 0;
                column = 0;
            }
        }
//       PORTA = (0x00 | (1 << column));
       PORTC = (0xFF & ~(1 << row));
       shiftWrite(0x00 | (1 << column));
//        shiftWrite(0xFF);
//        PORTC = 0x00;
//        if(i == 0)
//        {
//            shiftWrite(0x00);
//            i = 1;
//        }
//        else
//        {
//            shiftWrite(0xFF);
//            i = 0;
//        }
        while(!TimerFlag);
        TimerFlag = 0;
    }
}

