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
    DDRA = 0xFF;
    PORTA = 0x00;

//    unsigned short data;
//    unsigned short i;
    //set period timer
    TimerSet(250);
    //initialize timer
    TimerOn();
    //enable interupts
//    sei();
    //enable keyboard
//    Keyboard_Init();
    //gets the key code from the keyboard
    unsigned char row = 0;
    unsigned char column = 0;
    while (1)
    {
//        buttons = GET_BUTTON;
//        ToneSM();
//        data = DATA;
//        PORTB = data;
//        if(char_waiting)
//        {
//            key_code = read_char();
//            PORTA = key_code;
//        }
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
       PORTA = (0x00 | (1 << column));
       PORTC = (0xFF & ~(1 << row));
//        PORTA = 0xFF;
//        PORTC = 0x00;
        //PORTB = 0xFF;
        while(!TimerFlag);
        TimerFlag = 0;
    }
}

