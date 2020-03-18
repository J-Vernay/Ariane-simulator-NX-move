#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include "widgets/glscenewidget.hpp"
#include "widgets/mazemapwidget.hpp"
#include "models/maze.h"

/**
 * @brief Classe principale de la logique de jeu
 */
class GameLogic
{
    GLSceneWidget * mSceneWidget;
    MazeMapWidget * mMapWidget;
    Maze mMaze;

public:
    GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget);

    Maze * getMazePtr() { return &mMaze; };
};

#endif // GAMELOGIC_HPP
