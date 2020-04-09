#include "goalitem.hpp"

GoalItem::GoalItem(Maze * mazeptr, MazeMapWidget * minimapptr, double xpos, double ypos, QString texturePath)
    : AbstractItem(xpos, ypos, texturePath), mMaze(mazeptr), mMapWidget(minimapptr)
{

}

void GoalItem::onCollision()
{
    /*
     * Génération d'un nombre aléatoire, correspondant à un mur extérieur à supprimer
     * Exemple de numérotation :
     * 16/0  1  2  3  4  5/6
     * 17                7
     * 18                8
     * 10/19 11 12 13 14 9/15
    */
    int width = mMaze->getWidth(), height = mMaze->getHeight();
    int exitId = rand() % (2 * (width + height));

    // Mur extérieur nord
    if(exitId < width)
    {
        mMaze->getCell(exitId, 0).setFrontier(Cell::N, false);
    }

    // Mur extérieur est
    else if (exitId < width + height)
    {
        mMaze->getCell(width - 1, exitId - width).setFrontier(Cell::E, false);
    }

    // Mur extérieur sud
    else if (exitId < 2 * width + height)
    {
        mMaze->getCell(exitId - width - height, height - 1).setFrontier(Cell::S, false);
    }

    // Mur extérieur ouest
    else
    {
        mMaze->getCell(0, exitId - 2 * width - height).setFrontier(Cell::W, false);
    }

    // Mis à jour de la minimap
    mMapWidget->update();
}
