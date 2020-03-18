#ifndef PLAYER_HPP
#define PLAYER_HPP


class Player
{
    double mPosX, mPosY;
    double mAngle;

public:
    Player(double posx = 0.5, double poxy = 0.5, double angle = 0.0);

    void setPosition(double posx, double posy) { mPosX = posx; mPosY = posy; };
    double getPosX() { return mPosX; };
    double getPosY() { return mPosY; };
};

#endif // PLAYER_HPP
