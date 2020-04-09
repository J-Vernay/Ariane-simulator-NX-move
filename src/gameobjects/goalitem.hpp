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
    Maze * mMaze;
    MazeMapWidget * mMapWidget;

public:
    GoalItem(Maze * mazeptr, MazeMapWidget * minimapptr, double xpos, double ypos, QString texturePath = QString(":/res/textures/logoTSE.png"));

    void onCollision() override;
};

#endif // GOALITEM_HPP
