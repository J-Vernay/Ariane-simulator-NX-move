#ifndef GOALITEM_HPP
#define GOALITEM_HPP

#include "abstractitem.hpp"
#include "maze.h"
#include "../widgets/mazemapwidget.hpp"

/**
 * @brief Objet à récupérer pour débloquer la sortie du labyrinthe
 */
class GoalItem : public AbstractItem
{
    Maze * mMaze;                   // Référence vers le labyrinthe
    MazeMapWidget * mMapWidget;     // Référence vers le plan du labyrinthe.

public:
    // Collision
    GoalItem(Maze * mazeptr, MazeMapWidget * minimapptr, double xpos, double ypos, QString texturePath = QString(":/res/textures/logoTSE.png"));

    // Redéfinition d'une méthode virtuelle pure
    //   Définie les actions à réalisée lorsque le joueur récupère l'objet 
    void onCollision() override;
};

#endif // GOALITEM_HPP
