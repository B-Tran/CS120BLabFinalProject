#ifndef _AI_H_
#define _AI_H_

//AI BEGIN
enum AIStates {AI_Start, AI_Init, AI_Wait, AI_Move, AI_Stop};

int AISM(int AIState)
{
    char canMove = ((rand()+randNum) % 2);
    //transitions
    switch(AIState)
    {
        case AI_Start:
            AIState = AI_Init;
            break;
        case AI_Init:
            AIState = AI_Wait;
            break;
        case AI_Wait:
            if(reset)
            {
                AIState = AI_Init;
            }
            else if(AIMode == 0x01 && begin)
            {
                AIState = AI_Move;
            }
            else
            {
                AIState = AI_Wait;
            }
            break;
        case AI_Move:
            if(!BallLost)
            {
                AIState = AI_Move;
            }
            else
            {
                AIState = AI_Stop;
            }
            break;
        case AI_Stop:
            AIState = AI_Init;
            break;
        default:
            AIState = AI_Start;
            break;
    }
    //actions
    switch(AIState)
    {
        case AI_Start:
            break;
        case AI_Init:
            AIButtons = 0x00;
            break;
        case AI_Wait:
            break;
        case AI_Move:
            if(AICenter > Bally && AICenter > AITop && canMove)
            {
                AIButtons = 0x01;
            }
            else if(AICenter < Bally && AICenter < AIBottom && canMove)
            {
                AIButtons = 0x02;
            }
            else
            {
                AIButtons = 0x00;
            }
            break;
        case AI_Stop:
            AIButtons = 0x00;
            break;
        default:
            break;
    }
    return AIState;
}
//AI END

#endif
