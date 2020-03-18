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

public:
    GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget);
};

#endif // GAMELOGIC_HPP
