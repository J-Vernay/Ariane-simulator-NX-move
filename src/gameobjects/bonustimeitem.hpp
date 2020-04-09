#ifndef BONUSTIMEITEM_HPP
#define BONUSTIMEITEM_HPP

#include "abstractitem.hpp"
#include "../widgets/timerwidget.hpp"

class BonusTimeItem : public AbstractItem
{
    TimerWidget * mTimerWidget;
protected:
    int deltaMs = 20 * 1000;
public:
    BonusTimeItem(TimerWidget * timerWidget, double xpos, double ypos, QString texturePath = QString(":/res/textures/hourglass.png"));

    void onCollision() override;
};

#endif // BONUSTIMEITEM_HPP
