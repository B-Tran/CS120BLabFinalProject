#ifndef _SHIFT_REGISTER_2_H_
#define _SHIFT_REGISTER_2_H_
#include <avr/io.h>
#include <util/delay.h>

//This Header is used for outputting to an HC595 shift register.
//This can be modified to work with other shift registers.

//definitions-----------------------
#define OUTPUT2_PORT PORTB   //port to output to shift register
#define OUTPUT2_DDR DDRB
#define DS2_POS PB3          //position of the data pin(DS)
#define SH2_CP_POS PB4       //position of the shift clock(SH_CP)
#define ST2_CP_POS PB5       //position of the store clock(ST_CP)
//----------------------------------

void shift2Init()
{
   //Make the Data(DS), Shift clock (SH_CP), Store Clock (ST_CP) lines output
   OUTPUT2_DDR|=((1<<SH2_CP_POS)|(1<<ST2_CP_POS)|(1<<DS2_POS));
}
// change data (DS)lines
#define Shift2DataHigh() (OUTPUT2_PORT|=(1<<DS2_POS))
#define Shift2DataLow() (OUTPUT2_PORT&=(~(1<<DS2_POS)))
//Sends a clock pulse on SH_CP line
void shift2Pulse()
{
   //Pulse the Shift Clock
   OUTPUT2_PORT|=(1<<SH2_CP_POS);//set shift clock to HIGH
   OUTPUT2_PORT&=(~(1<<SH2_CP_POS));//set shift clock to LOW
}
//Sends a clock pulse on ST_CP line
void shift2Latch()
{
   //Pulse the Store Clock
   OUTPUT2_PORT|=(1<<ST2_CP_POS);//set store clock HIGH
   _delay_loop_1(1);
   OUTPUT2_PORT&=(~(1<<ST2_CP_POS));//set store clock LOW
   _delay_loop_1(1);
}
/*
 * Main function that writes a single byte
 * to the output shift register
 *
 * Arguments:
 *      data: a single byte of data to be written to the shift register
 *
 * Returns:
 *      NONE
 *
 * Description:
 *      The byte of date is serially transferred to the shift register
 *      and then latched. The byte is the available on the outputs of 
 *      the shift register[7:0] IC
*/
void shift2Write(uint8_t data)
{
    //send each of the 8 bits serially
    //in the order of Most Significant Bit(MSB)
   for(uint8_t i=0;i<8;i++)
   {
      //Output the data on DS line according to the
      //Value of MSB
      if(data & 0b10000000)
      {
         //MSB is 1 so output high
         Shift2DataHigh();
      }
      else
      {
         //MSB is 0 so output low
         Shift2DataLow();
      }
      shift2Pulse();  //Pulse the Clock line
      data=data<<1;  //Now bring next bit at MSB position
   }
   //Now all 8 bits have been transferred to shift register
   //Move them to output latch 
   shift2Latch();
}
/*
Simple Delay function approx 0.5 seconds
used when there is no timer implementation
*/
void Wait2()
{
   for(uint8_t i=0;i<50;i++)
   {
      _delay_loop_2(0);
   }
}
#endif //_SHIFT_REGISTER_1_H_
