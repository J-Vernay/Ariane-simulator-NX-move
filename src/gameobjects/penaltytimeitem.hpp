#ifndef PENALTYTIMEITEM_HPP
#define PENALTYTIMEITEM_HPP

#include "bonustimeitem.hpp"

class PenaltyTimeItem : public BonusTimeItem
{
public:
    PenaltyTimeItem(TimerWidget * timerWidget, double xpos, double ypos, double radius = 0.1, QString texturePath = QString(":/res/textures/hourglass.png"))
        : BonusTimeItem(timerWidget, xpos, ypos, radius, texturePath){ deltaMs *= -1; mIsBad = true; };
};

#endif // PENALTYTIMEITEM_HPP
