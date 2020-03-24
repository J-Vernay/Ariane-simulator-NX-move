#include "gamelogic.hpp"

#include <QDebug>

const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 8;

bool GameLogic::isForbidden(int oldX, int oldY, int newX, int newY)
{
    // Welcome to hell

    bool madeOOB = (      newX < 0
                       || newY < 0
                       || newX > MAZE_WIDTH - 1
                       || newY > MAZE_HEIGHT - 1);

    bool wentThroughWall = !madeOOB && (
                               (newX < oldX && mMaze.getCell(newX, newY).isFrontier(Cell::E))
                            || (newX > oldX && mMaze.getCell(newX, newY).isFrontier(Cell::W))
                            || (newY < oldY && mMaze.getCell(newX, newY).isFrontier(Cell::S))
                            || (newY > oldY && mMaze.getCell(newX, newY).isFrontier(Cell::N)));

    return madeOOB || wentThroughWall;
}

GameLogic::GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget)
    : mSceneWidget(openGLSceneWidget), mMapWidget(miniMapWidget), mMaze(MAZE_WIDTH, MAZE_HEIGHT)
{
    mMaze.generate();

    mPlayer.setPosition(0.5, 0.5);
    mMapWidget->setPlayer(&mPlayer);
    mMapWidget->setMaze(&mMaze);
}

void GameLogic::movePlayer(GameLogic::Direction direction)
{
    int formerX = std::floor(mPlayer.getPosX());
    int formerY = std::floor(mPlayer.getPosY());

    switch(direction)
    {
        case Direction::FORWARD:
            mPlayer.goForward();

            // Verification des collisions
            if (isForbidden(formerX, formerY, std::floor(mPlayer.getPosX()), std::floor(mPlayer.getPosY())))
            {
                mPlayer.goBackward();
            }
            qDebug() << mPlayer.getPosY();
            break;

        case Direction::BACKWARD:
            mPlayer.goBackward();
            break;
        case Direction::LEFT:
            mPlayer.turnLeft();
            break;
        case Direction::RIGHT:
            mPlayer.turnRight();
            break;
    }
    mMapWidget->update();
}
