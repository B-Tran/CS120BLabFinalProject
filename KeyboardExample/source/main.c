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
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

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

//gets the keyboard data
volatile uint8_t kbd_data;
//this checks when a character is ready to be read
volatile uint8_t char_waiting;
//this checks for the start bit on the DATA line
uint8_t started;
//this reads checks the number of data bits being read
uint8_t bit_count;
//this checks if the shift key was pressed
uint8_t shift;
//this checks if caps lock was pressed
uint8_t caps_lock;
//this checks if a special character was pressed
uint8_t extended;
//this checks if we are in release mode
uint8_t release;
//this gets the parity bit
uint8_t parity;

//initialize flags to allow interupts on port B
void Keyboard_Init(void)
{
    //initialize all needed values
    started = 0;
    kbd_data = 0;
    bit_count = 0;
    parity = 0;

    /*
     * make PB4 the input pin
     * DDRC &= !(1 << PB4);
     * turn on pullup resistor
     * PORTC |= (1 << PB4);
     */
    //setting the status register to allow for
    //enabling of interupts on bit I
    //SREG |= (1 << 0x07);
    //*can be initialized in main using sei()

    //allow for pin change interupts a certain range
    //in this case, PCINT[14:8]
    PCICR |= (1 << PCIE1);
    //allows for the selecti559-978-8221on of which pins to be interupt
    //enabled for, PCINT[14:8]
    //in this case, enabling PCINT12 for interupt
    PCMSK1 |= (1 << PCINT12);
}

//an isr where it takes the interupt vector to check
//for data on the interupt pin 
ISR(PCINT1_vect)
{
    //make sure clock line is low, if not ignore this transmisson
    if(PINB & (1 << PB4) )
    {
        return;
    }

    //if the reading is not started, check for the start bit on DATA line
    if(!started)
    {
        //check if the data line is set low
        //and reset the bit count and keyboard data
        if( (PINB & (1 << PB2)) == 0 )
        {
            started = 1;
            bit_count = 0;
            kbd_data = 0;
            return;
        }
    }
    //if the reading has started, read in the new bit and
    //add the ones to the corresponding places of the kdb_data
    //stop the read once 8 bits have been read
    else if(bit_count < 8)
    {
        if(PINB & (1 << PB2))
        {
            kbd_data |= (1 << bit_count);
        }
        bit_count++;
        return;
    }
    //gets the parity bit
    else if(bit_count == 8)
    {
        parity = (PINB & (1 << PB2)) ? 1:0;
        bit_count++;
        return;
    }
    //stop bit
    //should check to make sure data line is high
    else
    {
        started = 0;
        bit_count = 0;
    }

    //checks for release code
    if(kbd_data == 0xF0)
    {
        release = 1;
        kbd_data = 0;
        return;
    }
    //checks for shift key
    else if(kbd_data == 0x12)
    {
        if(release == 0)
        {
            shift = 1;
        }
        else
        {
            shift = 0;
            release = 0;
        }
        return;
    }
    //checks if not one of those special characters
    else
    {
        if(release == 1)
        {
            release = 0;
        }
        else
        {
            char_waiting = 1;
        }
    }
}

//gets a character
uint8_t read_char()
{
    //waits until a character is read
    while(!char_waiting);
    //reset char_waiting
    char_waiting = 0;
    return kbd_data;
}

int main(void) {
    //inputs
    //reading of butttons
    //DDRC = 0x02;
    //PORTC = 0x01;
    
    //set PB4 to input pullup resistor and PB2 as readin
    DDRB = 0x00 | (1 << PB2);
    PORTB = 0x00 | (1 << PB4);

    //set port A as output
    DDRA = 0xFF;
    PORTA = 0x00;

//    unsigned short data;
//    unsigned short i;
    //set period timer
//    TimerSet(1000);
    //initialize timer
//    TimerOn();
    //enable interupts
//    sei();
    //enable keyboard
//    Keyboard_Init();
    //gets the key code from the keyboard
    uint8_t key_code = 0;
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
        PORTA = 0xFF;
        //PORTB = 0xFF;
        //while(!TimerFlag);
        //TimerFlag = 0;
    }
}

