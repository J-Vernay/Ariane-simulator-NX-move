#ifndef PENALTYTIMEITEM_HPP
#define PENALTYTIMEITEM_HPP

#include "bonustimeitem.hpp"

class PenaltyTimeItem : public BonusTimeItem
{
public:
    PenaltyTimeItem(TimerWidget * timerWidget, double xpos, double ypos, QString texturePath = QString(":/res/textures/hourglass.png"))
        : BonusTimeItem(timerWidget, xpos, ypos, texturePath){ deltaMs *= -1; mIsBad = true; };
};

#endif // PENALTYTIMEITEM_HPP
