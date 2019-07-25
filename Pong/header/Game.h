#ifndef _GAME_H_
#define _GAME_H_
//GAME BEGIN
enum GameStates {Game_Start, Game_Init, Game_Wait, Game_Begin,
                        Game_Reset, Game_Reset_Done};

int GameSM(int GameState)
{
    uint8_t buttons = GET_BUTTONSSR;
    uint8_t AISwitch = GET_AI;
    //transitions
    switch(GameState)
    {
        case Game_Start:
            GameState = Game_Init;
            break;
        case Game_Init:
            GameState = Game_Wait;
            break;
        case Game_Wait:
            if(buttons == 0x01 && !begin)
            {
                AIMode =((AIMode && !RoundDone) ||
                        ((AISwitch == 0x01) && RoundDone)) ? 0x01:0x00;
                GameState = Game_Begin;
            }
            else if(buttons == 0x02)
            {
                GameState = Game_Reset;
            }
            else
            {
                GameState = Game_Wait;
            }
            break;
        case Game_Begin:
            if(buttons == 0x00)
            {
                GameState = Game_Wait;
                begin = 0x01;
                reset = 0x00;
            }
            else
            {
                GameState = Game_Begin;
            }
            break;
        case Game_Reset:
            if(buttons == 0x00)
            {
                GameState = Game_Reset_Done;
                begin = 0x00;
                reset = 0x01;
            }
            else
            {
                GameState = Game_Reset;
            }
            break;
        case Game_Reset_Done:
            GameState = Game_Init;
            break;
        default:
            GameState = Game_Start;
            break;
    }
    //actions
    switch(GameState)
    {
        case Game_Start:
            break;
        case Game_Init:
            begin = 0x00;
            reset = 0x00;
            AIMode = 0x00;
            break;
        case Game_Wait:
            break;
        case Game_Reset:
            break;
        case Game_Reset_Done:
            break;
        default:
            break;
    }
    return GameState;
}
//GAME END

#endif
