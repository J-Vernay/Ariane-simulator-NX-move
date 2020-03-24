#include "player.hpp"

Player::Player(double posx, double posy, double angle)
    : mPosX(posx), mPosY(posy), mAngle(angle)
{

}

void Player::goForward()
{
    mPosX += std::cos(mAngle) * mWalkSpeed;
    mPosY += std::sin(mAngle) * mWalkSpeed;
}

void Player::goBackward()
{
    mPosX += -1 * std::cos(mAngle) * mWalkSpeed;
    mPosY += -1 * std::sin(mAngle) * mWalkSpeed;
}

void Player::turnLeft()
{
    mAngle -= mRotateSpeed;
}

void Player::turnRight()
{
    mAngle += mRotateSpeed;
}
