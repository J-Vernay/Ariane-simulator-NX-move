#include "gamelogic.hpp"

const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 8;

GameLogic::GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget)
    : sceneWidget(openGLSceneWidget), mapWidget(miniMapWidget), maze(MAZE_WIDTH, MAZE_HEIGHT)
{

}
