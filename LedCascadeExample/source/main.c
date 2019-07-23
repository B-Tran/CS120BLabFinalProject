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

//#define CLOCK (PINC & 0x01)
//#define DATA (PORTC & 0x02)

typedef struct _lights
{
    uint8_t row;
    uint8_t column;
} lights;

const lights l_list[][8] =
{
    { 
        {0x01, 0x01},
        {0x02, 0x02},
        {0x04, 0x04},
        {0x08, 0x08},
        {0x10, 0x10},
        {0x20, 0x20},
        {0x40, 0x40},
        {0x80, 0x80}
    },

    { 
        {0x02, 0x01},
        {0x04, 0x02},
        {0x08, 0x04},
        {0x10, 0x08},
        {0x20, 0x10},
        {0x40, 0x20},
        {0x80, 0x40},
        {0x00, 0x00}
    },

    { 
        {0x04, 0x01},
        {0x08, 0x02},
        {0x10, 0x04},
        {0x20, 0x08},
        {0x40, 0x10},
        {0x80, 0x20},
        {0x00, 0x00},
        {0x00, 0x00}
    },

    { 
        {0x08, 0x01},
        {0x10, 0x02},
        {0x20, 0x04},
        {0x40, 0x08},
        {0x80, 0x10},
        {0x00, 0x00},
        {0x00, 0x00},
        {0x00, 0x00}
    },

    { 
        {0x10, 0x01},
        {0x20, 0x02},
        {0x40, 0x04},
        {0x80, 0x08},
        {0x00, 0x00},
        {0x00, 0x00},
        {0x00, 0x00},
        {0x00, 0x00}
    },

    { 
        {0x20, 0x01},
        {0x40, 0x02},
        {0x80, 0x04},
        {0x00, 0x08},
        {0x00, 0x10},
        {0x00, 0x20},
        {0x00, 0x40},
        {0x00, 0x80}
    },

    { 
        {0x40, 0x01},
        {0x80, 0x02},
        {0x00, 0x04},
        {0x00, 0x08},
        {0x00, 0x10},
        {0x00, 0x20},
        {0x00, 0x40},
        {0x00, 0x80}
    },

    { 
        {0x80, 0x01},
        {0x00, 0x02},
        {0x00, 0x04},
        {0x00, 0x08},
        {0x00, 0x10},
        {0x00, 0x20},
        {0x00, 0x40},
        {0x00, 0x80}
    },

    { 
        {0x00, 0x01},
        {0x00, 0x02},
        {0x00, 0x04},
        {0x00, 0x08},
        {0x00, 0x10},
        {0x00, 0x20},
        {0x00, 0x40},
        {0x00, 0x80}
    },
};

int main(void) {
//    //output of columns
//    DDRC = 0xFF;
//    PORTC = 0x00;
    
    //set port A as output columns
//    DDRA = 0xFF;
//    PORTA = 0x00;

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
    while (1)
    {
        if(state == 0)
        {
            shift2Write(0xFF);
            shiftWrite(0x00);
            state = 1;
            ++i;
        }
        else
        {
            shift2Write(0xFF & (~(l_list[j][i].row)) );
            shiftWrite(0x00 | (l_list[j][i].column));
            state = 0;
            
        }
        if(i >= 8)
        {
            i = 0;
        }
        if(k < 500)
        {
            ++k;
        }
        else
        {
            k=0;
            if(j < 8)
            {
                j++;
            }
            else
            {
                j = 0;
            }
        }

        while(!TimerFlag);
        TimerFlag = 0;
    }
}

