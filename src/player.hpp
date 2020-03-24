#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cmath>

class Player
{
    double mPosX, mPosY;
    double mAngle;
    const double mWalkSpeed = 0.2;
    const double mRotateSpeed = M_PI / 30.0;

public:
    Player(double posx = 0.5, double poxy = 0.5, double angle = 0.0);

    void setPosition(double posx, double posy) { mPosX = posx; mPosY = posy; };
    double getPosX() { return mPosX; };
    double getPosY() { return mPosY; };
    double getAngle() { return mAngle; };

    void goForward();
    void goBackward();
    void turnLeft();
    void turnRight();
};

#endif // PLAYER_HPP
