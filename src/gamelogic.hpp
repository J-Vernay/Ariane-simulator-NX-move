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

    bool isForbidden(int oldX, int oldY, int newX, int newY);

public:
    GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget);

    enum Direction{FORWARD, BACKWARD, LEFT, RIGHT};
    void movePlayer(Direction direction);
};

#endif // GAMELOGIC_HPP
