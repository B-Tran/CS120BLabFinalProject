#ifndef _PLAYER_1_H_
#define _PLAYER_1_H_
//PLAYER 1 BEGIN
enum Player1States {Player1_Start, Player1_Init, Player1_Wait, Player1_Up,
    Player1_Down} Player1State;
void Player1SM()
{
    //gets button input
    uint8_t buttons = GET_BUTTONS;
    //which side the paddle is on
    static const uint8_t colPos = 7;
    //sets the postion of paddle
    static uint8_t top;
    static uint8_t middle;
    static uint8_t bottom;
    //transitions
    switch(Player1State)
    {
        case Player1_Start:
            Player1State = Player1_Init;
            break;
        case Player1_Init:
            Player1State = Player1_Wait;
            break;
        case Player1_Wait:
            if(buttons == 0x01)
            {
                Player1State = Player1_Up;
            }
            else if(buttons == 0x02)
            {
                Player1State = Player1_Down;
            }
            else
            {
                Player1State = Player1_Wait;
            }
            break;
        case Player1_Up:
            Player1State = Player1_Wait;
            break;
        case Player1_Down:
            Player1State = Player1_Wait;
            break;
        default:
            Player1State = Player1_Init;
            break;
    }
    //actions
    switch(Player1State)
    {
        case Player1_Start:
            break;
        case Player1_Init:
            top = 2;
            middle = 3;
            bottom = 4;
            break;
        case Player1_Wait:
            DisplayArray[top][colPos] = 1;
            DisplayArray[middle][colPos] = 1;
            DisplayArray[bottom][colPos] = 1;
            break;
        case Player1_Up:
            if(top > 0)
            {
                DisplayArray[bottom][colPos] = 0;
                top--;
                middle--;
                bottom--;
                DisplayArray[top][colPos] = 1;
                DisplayArray[middle][colPos] = 1;
                DisplayArray[bottom][colPos] = 1;
            }
            break;
        case Player1_Down:
            if(bottom < 7)
            {
                DisplayArray[top][colPos] = 0;
                top++;
                middle++;
                bottom++;
                DisplayArray[top][colPos] = 1;
                DisplayArray[middle][colPos] = 1;
                DisplayArray[bottom][colPos] = 1;
            }
            break;
        default:
            break;
    }
}
//PLAYER 1 END
#endif
