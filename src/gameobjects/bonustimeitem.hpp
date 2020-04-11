#ifndef BONUSTIMEITEM_HPP
#define BONUSTIMEITEM_HPP

#include "abstractitem.hpp"
#include "../widgets/timerwidget.hpp"

/**
 * @brief Classe de l'objet "bonus de temps"
 */
class BonusTimeItem : public AbstractItem
{
    TimerWidget * mTimerWidget;     // Référence vers le chronomètre, afin de modifier sa valeur

protected:
    int deltaMs = 20 * 1000;        // Nombre de millisecondes à retirer du temps de jeu
    
public:
    // Constructeur. Assigne par défaut une texture de sablier à l'objet
    BonusTimeItem(TimerWidget * timerWidget, double xpos, double ypos, QString texturePath = QString(":/res/textures/hourglass.png"));

    // Redéfinition d'une méthode virtuelle pure
    //   Définie les actions à réalisée lorsque le joueur récupère l'objet 
    void onCollision() override;
};

#endif // BONUSTIMEITEM_HPP
