#ifndef _BALL_BOUNCE_H_
#define _BALL_BOUNCE_H_

//BALL BEGIN
enum BallStates {Ball_Start, Ball_Init, Ball_Wait, Ball_Bounce,
                    Ball_Stop} BallState;

void BallSM()
{
    static const uint8_t cntMax = 8;
    static const uint8_t cntMin = 4;
    static uint8_t cntTop;
    static uint8_t cnt;
    uint8_t tempX;
    uint8_t tempY;
    static int8_t ballDirX;
    static int8_t ballDirY;
    //transitions
    switch(BallState)
    {
        case Ball_Start:
            BallState = Ball_Init;
            break;
        case Ball_Init:
            BallState = Ball_Wait;
            break;
        case Ball_Wait:
            if(begin)
            {
                BallState = Ball_Bounce;
            }
            else
            {
                BallState = Ball_Wait;
            }
            break;
        case Ball_Bounce:
            if(reset)
            {
                BallState = Ball_Stop;
            }
            else if(!BallLost && !reset)
            {
                BallState = Ball_Bounce;
            }
            else
            {
                BallState = Ball_Stop;
            }
            break;
        case Ball_Stop:
            //BallState = Ball_Init;
            BallState = Ball_Init;
            break;
        default:
            BallState = Ball_Start;
            break;
    }
    //actions
    switch(BallState)
    {
        case Ball_Start:
            break;
        case Ball_Init:
            Ballx = 4 - ((rand()+randNum) % 2);
            Bally = 4 - ((rand()+randNum) % 2);
            ballDirX = (((rand()+randNum) % 2) ? 1:-1);
            ballDirY = (((rand()+randNum) % 2) ? 1:-1);
            BallLost = 0;
            DisplayArray[Bally][Ballx] = 1;
            cnt = 0;
            cntTop = cntMax;
            break;
        case Ball_Wait:
            DisplayArray[Bally][Ballx] = 1;
            break;
        case Ball_Bounce:
            if(cnt < cntTop)
            {
                cnt++;
                return;
            }
            else
            {
                cnt = 0;
            }
            tempX = Ballx;
            tempY = Bally;
            //if ball y position is decrementing and ball has not hit wall
            if(ballDirY == -1 && Bally > 0)
            {
                Bally += ballDirY;
            }
            else if(ballDirY == -1 && Bally == 0)
            {
                ballDirY = 1;
                Bally += ballDirY;
            }
            //if ball y position is incrementing
            else if(ballDirY == 1 && Bally < (MATRIX_SIZE-1))
            {
                Bally += ballDirY;
            }
            else
            {
                ballDirY = -1;
                Bally += ballDirY;
            }
            
            if(ballDirX == -1 && Ballx > 1)
            {
                Ballx += ballDirX;
            }
            else if(ballDirX == -1 && Ballx == 1 && 
                    DisplayArray[Bally][Ballx-1] == 1)
            {
                ballDirX = 1;
                if(ballDirY == -1 && !DisplayArray[Bally+1][Ballx-1])
                {
                    ballDirY = 1;
                    Bally = tempY + ballDirY;
                    cntTop = cntMin; 
                }
                else if(ballDirY == 1 && !DisplayArray[Bally-1][Ballx-1])
                {
                    ballDirY = -1;
                    Bally = tempY + ballDirY;
                    cntTop = cntMin; 
                }
                else
                {
                    cntTop = cntMax;
                }
                Ballx += ballDirX;
            }
            else if(ballDirX == 1 && Ballx < (MATRIX_SIZE-2))
            {
                Ballx += ballDirX;
            }
            else if(ballDirX == 1 && Ballx == (MATRIX_SIZE-2) && 
                    DisplayArray[Bally][Ballx+1] == 1)
            {
                ballDirX = -1;
                if(ballDirY == -1 && !DisplayArray[Bally+1][Ballx+1])
                {
                    ballDirY = 1;
                    Bally = tempY + ballDirY;
                    cntTop = cntMin; 
                }
                else if(ballDirY == 1 && !DisplayArray[Bally-1][Ballx+1])
                {
                    ballDirY = -1;
                    Bally = tempY + ballDirY;
                    cntTop = cntMin; 
                }
                else
                {
                    cntTop = cntMax;
                }
                Ballx += ballDirX;
            }
            else
            {
                BallLost = 1;
            }
//            Ballx += ballDirX;

            if(!BallLost)
            {
                DisplayArray[tempY][tempX] = 0;
                DisplayArray[Bally][Ballx] = 1;
            }
            else
            {
                DisplayArray[tempY][tempX] = 0;
                DisplayArray[Bally][Ballx] = 0;
                BallPoint = 1;
            }
            break;
        case Ball_Stop:
            cnt = 0;
            cntTop = cntMax;
            DisplayArray[Bally][Ballx] = 0;
            begin = 0x00;
            BallLost = 0x00;
            break;
        default:
            break;
    }
}
//BALL END

#endif
