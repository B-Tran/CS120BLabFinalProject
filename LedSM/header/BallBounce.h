#ifndef _BALL_BOUNCE_H_
#define _BALL_BOUNCE_H_

//BALL BEGIN
enum BallStates {Ball_Start, Ball_Init, Ball_Bounce} BallState;

void BallSM()
{
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
            BallState = Ball_Bounce;
            break;
        case Ball_Bounce:
            BallState = Ball_Bounce;
            break;
    }
    //actions
    switch(BallState)
    {
        case Ball_Start:
            break;
        case Ball_Init:
            Ballx = 4;
            Bally = 4;
            ballDirX = 1;//((rand() % 2) ? 1:-1);
            ballDirY = 1;//((rand() % 2) ? 1:-1);
            BallLost = 0;
            DisplayArray[Bally][Ballx] = 1;
            break;
        case Ball_Bounce:
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
 //           Bally += ballDirY;
            
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
                }
                else if(ballDirY == 1 && !DisplayArray[Bally-1][Ballx-1])
                {
                    ballDirY = -1;
                    Bally = tempY + ballDirY;
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
                }
                else if(ballDirY == 1 && !DisplayArray[Bally-1][Ballx+1])
                {
                    ballDirY = -1;
                    Bally = tempY + ballDirY;
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
            }
            break;
    }
}
//BALL END

#endif
