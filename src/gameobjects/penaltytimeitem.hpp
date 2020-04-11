#ifndef PENALTYTIMEITEM_HPP
#define PENALTYTIMEITEM_HPP

#include "bonustimeitem.hpp"

/**
 * @brief Classe de l'objet "malus de temps".
 */
class PenaltyTimeItem : public BonusTimeItem
{
    // Note : Cette classe dérive de BonusTimeItem, et impose uniquement une valeur négative à deltaMs
public:
    // Constructeur
    PenaltyTimeItem(TimerWidget * timerWidget, double xpos, double ypos, QString texturePath = QString(":/res/textures/hourglass.png"))
        : BonusTimeItem(timerWidget, xpos, ypos, texturePath){ deltaMs *= -1; mIsBad = true; };
};

#endif // PENALTYTIMEITEM_HPP
