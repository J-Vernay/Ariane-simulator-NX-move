#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include "widgets/glscenewidget.hpp"
#include "widgets/mazemapwidget.hpp"
#include "models/maze.h"
#include "player.hpp"

/**
 * @brief Classe principale de la logique de jeu
 */
class GameLogic
{
    GLSceneWidget * mSceneWidget;
    MazeMapWidget * mMapWidget;
    Maze mMaze;
    Player mPlayer;

    // Appeler après un déplacement logique. Cette méthode va corriger le déplacement du
    //      joueur ssi ce dernier est entré en collision avec un mur
    void handleWallCollisions(double oldX, double oldY, double newX, double newY);

public:
    GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget);

    // Fonction de déplacement du joueur, dans une direction définie par l'enum Direction.
    enum Direction{FORWARD, BACKWARD, LEFT, RIGHT, STOP};
    void movePlayer(Direction direction);
};

#endif // GAMELOGIC_HPP
