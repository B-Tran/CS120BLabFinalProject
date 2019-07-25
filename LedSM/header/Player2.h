#ifndef _PLAYER_2_H_
#define _PLAYER_2_H_
//PLAYER 2 BEGIN
enum Player2States {Player2_Start, Player2_Init, Player2_Wait, Player2_Up,
    Player2_Down, Player2_Reset} Player2State;
void Player2SM()
{
    //gets button input
    uint8_t buttons;
    if(AIMode == 0x01)
    {
        buttons = AIButtons;
    }
    else
    {
        //fix later
        buttons = GET_BUTTONSP2;
    }
    //which side the paddle is on
    static const uint8_t colPos = 0;
    //sets the postion of paddle
    static uint8_t top;
    static uint8_t middle;
    static uint8_t bottom;
    //transitions
    switch(Player2State)
    {
        case Player2_Start:
            Player2State = Player2_Init;
            break;
        case Player2_Init:
            Player2State = Player2_Wait;
            break;
        case Player2_Wait:
            if(reset)
            {
                Player2State = Player2_Reset;
            }
            else if(buttons == 0x01 && begin && !reset && !BallLost)
            {
                Player2State = Player2_Up;
            }
            else if(buttons == 0x02 && begin && !reset && !BallLost)
            {
                Player2State = Player2_Down;
            }
            else if(BallLost)
            {
                Player2State = Player2_Reset;
            }
            else
            {
                Player2State = Player2_Wait;
            }
            break;
        case Player2_Up:
            Player2State = Player2_Wait;
            break;
        case Player2_Down:
            Player2State = Player2_Wait;
            break;
        case Player2_Reset:
            Player2State = Player2_Wait;
            break;
        default:
            Player2State = Player2_Init;
            break;
    }
    //actions
    switch(Player2State)
    {
        case Player2_Start:
            break;
        case Player2_Init:
            top = 2;
            middle = 3;
            bottom = 4;
            AICenter = middle;
            break;
        case Player2_Wait:
            DisplayArray[top][colPos] = 1;
            DisplayArray[middle][colPos] = 1;
            DisplayArray[bottom][colPos] = 1;
            break;
        case Player2_Up:
            if(top > 0)
            {
                DisplayArray[bottom][colPos] = 0;
                top--;
                middle--;
                bottom--;
                AICenter = middle;
                DisplayArray[top][colPos] = 1;
                DisplayArray[middle][colPos] = 1;
                DisplayArray[bottom][colPos] = 1;
            }
            break;
        case Player2_Down:
            if(bottom < 7)
            {
                DisplayArray[top][colPos] = 0;
                top++;
                middle++;
                bottom++;
                AICenter = middle;
                DisplayArray[top][colPos] = 1;
                DisplayArray[middle][colPos] = 1;
                DisplayArray[bottom][colPos] = 1;
            }
            break;
        case Player2_Reset:
            DisplayArray[top][colPos] = 0;
            DisplayArray[middle][colPos] = 0;
            DisplayArray[bottom][colPos] = 0;
            top = 2;
            middle = 3;
            bottom = 4;
            AICenter = middle;
            break;
        default:
            break;
    }
}
//PLAYER 2 END
#endif
