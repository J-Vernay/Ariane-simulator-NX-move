#include "gamelogic.hpp"

#include <QDebug>

const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 8;

GameLogic::GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget, TimerWidget * timerWidget)
    : mSceneWidget(openGLSceneWidget), mMapWidget(miniMapWidget), mTimerWidget(timerWidget),
      mMaze(MAZE_WIDTH, MAZE_HEIGHT)
{
    mMaze.generate();

    mPlayer.setPosition(0.5, 0.5);
    mMapWidget->setPlayer(&mPlayer);
    mMapWidget->setMaze(&mMaze);
    mSceneWidget->setMaze(&mMaze);
    mSceneWidget->setPlayer(&mPlayer);
}

void GameLogic::handleWallCollisions(double oldX, double oldY, double newX, double newY)
{
    // Calcul des index des cases pré et post déplacement
    int oldXi = std::floor(oldX);
    int oldYi = std::floor(oldY);
    int newXi = std::floor(newX);
    int newYi = std::floor(newY);

    // Booléens de collision
    bool collisionE = (newXi > oldXi && mMaze.getCell(oldXi, oldYi).isFrontier(Cell::E));
    bool collisionW = (newXi < oldXi && mMaze.getCell(oldXi, oldYi).isFrontier(Cell::W));
    bool collisionN = (newYi < oldYi && mMaze.getCell(oldXi, oldYi).isFrontier(Cell::N));
    bool collisionS = (newYi > oldYi && mMaze.getCell(oldXi, oldYi).isFrontier(Cell::S));

    // Booléen vrai si la nouvelle position est en dehors du labyrinthe
    bool oobNewPos = (newXi < 0 || newYi < 0 || newXi > MAZE_WIDTH - 1 || newYi > MAZE_HEIGHT - 1);

    // Booléens de collisions "inversés" (calculés à partir de la nouvelle position)
    bool rCollisionE = (!oobNewPos && oldXi < newXi && mMaze.getCell(newXi, newYi).isFrontier(Cell::W));
    bool rCollisionW = (!oobNewPos && oldXi > newXi && mMaze.getCell(newXi, newYi).isFrontier(Cell::E));
    bool rCollisionN = (!oobNewPos && newYi >= 0 && oldYi > newYi && mMaze.getCell(newXi, newYi).isFrontier(Cell::S));
    bool rCollisionS = (!oobNewPos && mMaze.getCell(newXi, newYi).isFrontier(Cell::N));

    // Collision dans un angle, à l'intérieur
    if ((collisionE || collisionW) && (collisionN || collisionS))
    {
        mPlayer.setPosition(oldX, oldY);
        return;
    }

    // Collision latérale
    if (collisionE || collisionW)
    {
        mPlayer.setPosition(oldX, newY);
        return;
    }

    // Collision verticale
    if (collisionN || collisionS)
    {
        mPlayer.setPosition(newX, oldY);
        return;
    }

    // Collision dans un angle, à l'extérieur
    if ((rCollisionE || rCollisionW) && (rCollisionN || rCollisionS))
    {
        mPlayer.setPosition(oldX, oldY);
    }
}

void GameLogic::movePlayer(GameLogic::Direction direction)
{
    double formerX = mPlayer.getPosX();
    double formerY = mPlayer.getPosY();

    switch(direction)
    {
        case Direction::FORWARD:
            mPlayer.goForward();
            handleWallCollisions(formerX, formerY, mPlayer.getPosX(), mPlayer.getPosY());
            break;
        case Direction::BACKWARD:
            mPlayer.goBackward();
            handleWallCollisions(formerX, formerY, mPlayer.getPosX(), mPlayer.getPosY());
            break;
        case Direction::LEFT:
            mPlayer.turnLeft();
            break;
        case Direction::RIGHT:
            mPlayer.turnRight();
            break;
        default:
            break;
    }
    mMapWidget->update();
    mSceneWidget->update();

    if (direction == Direction::STOP)
    {
        mMapWidget->show();
    }
    else
    {
        mMapWidget->hide();
    }
}
