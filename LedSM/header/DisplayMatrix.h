#ifndef _DISPLAY_MATRIX_H_
#define _DISPLAY_MATRIX_H_

//DISPLAY BEGIN
enum DisplayStates {Display_Start, Display_Init, Display_Print} 
        DisplayState;

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
        default:
            DisplayState = Display_Init;
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
        default:
            break;
    }
}
//DISPLAY END
#endif
