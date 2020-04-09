#include "bonustimeitem.hpp"

BonusTimeItem::BonusTimeItem(TimerWidget * timerWidget, double xpos, double ypos, QString texturePath)
    : AbstractItem(xpos, ypos, texturePath), mTimerWidget(timerWidget)
{

}

void BonusTimeItem::onCollision()
{
    if (mTimerWidget->getMs() > deltaMs)
        mTimerWidget->subTime(deltaMs);
    else
        mTimerWidget->subTime(mTimerWidget->getMs());
}
