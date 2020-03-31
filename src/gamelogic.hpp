#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include "widgets/glscenewidget.hpp"
#include "widgets/mazemapwidget.hpp"
#include "widgets/timerwidget.hpp"
#include "gameobjects/maze.h"
#include "gameobjects/player.hpp"
#include "gameobjects/abstractitem.hpp"

/**
 * @brief Classe principale de la logique de jeu
 */
class GameLogic
{
    GLSceneWidget * mSceneWidget;
    MazeMapWidget * mMapWidget;
    TimerWidget * mTimerWidget;

    Maze mMaze;
    Player mPlayer;

    std::vector<AbstractItem *> mItems;

public:
    GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget, TimerWidget * timerWidget);
    ~GameLogic();

    // Fonction de déplacement du joueur, dans une direction définie par l'enum Direction.
    enum Direction{FORWARD, BACKWARD, LEFT, RIGHT, STOP};
    void movePlayer(Direction direction);

private:
    // Appeler après un déplacement logique. Cette méthode va corriger le déplacement du
    //      joueur ssi ce dernier est entré en collision avec un mur
    void handleWallCollisions(double oldX, double oldY, double newX, double newY);
};

#endif // GAMELOGIC_HPP
