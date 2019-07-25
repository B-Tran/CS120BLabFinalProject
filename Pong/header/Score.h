#ifndef _SCORE_H_
#define _SCORE_H_

//SCORE BEGIN
enum ScoreStates {Score_Start, Score_Init, Score_Wait, Score_Point,
    Score_Check ,Score_Done};
void PrintWinMsg(char player)
{
    char message3[14] = "PlayerX Wins.";
    message3[6] = player;
    LCD_ClearScreen();
    for(uint8_t i = 0; i < 13; ++i)
    {
        LCD_Cursor(i+1);
        LCD_WriteData(message3[i]);
    }
}

void PrintPointMsg(uint8_t player1, uint8_t player2)
{
    char message1[12] = "Player1 : ";
    char message2[12] = "Player2 : ";
    message1[10] = (player1 + '0');
    message2[10] = (player2 + '0');
    for(uint8_t i = 0; i < 11; ++i)
    {
        LCD_Cursor(i+1);
        LCD_WriteData(message1[i]);
        LCD_Cursor(i+17);
        LCD_WriteData(message2[i]);
    }

}
int ScoreSM(int ScoreState)
{
    static uint8_t player1Pnt;
    static uint8_t player2Pnt;
    static const uint8_t maxPnt = 3;
    //transitions
    switch(ScoreState)
    {
        case Score_Start:
            ScoreState = Score_Init;
            break;
        case Score_Init:
            ScoreState = Score_Wait;
            break;
        case Score_Wait:
            if(reset)
            {
                ScoreState = Score_Init;
            }
            else if(begin && !reset)
            {
                RoundDone = 0x00;
                ScoreState = Score_Point;
                LCD_ClearScreen();
                PrintPointMsg(player1Pnt, player2Pnt);
            }
            else
            {
                ScoreState = Score_Wait;
            }
            break;
        case Score_Point:
            if(reset)
            {
                ScoreState = Score_Init;
            }
            else if(!BallPoint && !reset)
            {
                ScoreState = Score_Point;
            }
            else
            {
                if(Ballx >= (MATRIX_SIZE - 2))
                {
                    player2Pnt++;
                }
                else
                {
                    player1Pnt++;
                }
                ScoreState = Score_Check;
            }
            break;
        case Score_Check:
            if(reset)
            {
                ScoreState = Score_Init;
            }
            else if((player1Pnt < maxPnt) && (player2Pnt < maxPnt))
            {
                PrintPointMsg(player1Pnt, player2Pnt);
                ScoreState = Score_Wait;
            }
            else
            {
                ScoreState = Score_Done;
            }
            break;
        case Score_Done:
            ScoreState = Score_Wait;
        default:
            break;
    }
    //actions
    switch(ScoreState)
    {
        case Score_Start:
            break;
        case Score_Init:
            player1Pnt = 0;
            player2Pnt = 0;
            RoundDone = 1;
            LCD_ClearScreen();
            PrintPointMsg(player1Pnt, player2Pnt);
            break;
        case Score_Wait:
            BallPoint = 0;
            break;
        case Score_Point:
            break;
        case Score_Done:
            if(player1Pnt >= maxPnt)
            {
                PrintWinMsg('1');
            }
            else
            {
                PrintWinMsg('2');
            }
            player1Pnt = 0;
            player2Pnt = 0;
            RoundDone = 1;
            break;
        case Score_Check:
            break;
        default:
            break;
    }
    return ScoreState;
}
//SCORE BEGIN
#endif
