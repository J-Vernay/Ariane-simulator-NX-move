#include "gamelogic.hpp"

#include <QDebug>
#include <QMessageBox>

const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 8;

const int FPS = 30;

GameLogic::GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget, TimerWidget * timerWidget, QObject * parent)
    : QObject(parent), mSceneWidget(openGLSceneWidget), mMapWidget(miniMapWidget), mTimerWidget(timerWidget),
      mMaze(MAZE_WIDTH, MAZE_HEIGHT)
{ 
    mMapWidget->setPlayer(&mPlayer);
    mMapWidget->setMaze(&mMaze);
    mSceneWidget->setMaze(&mMaze);
    mSceneWidget->setPlayer(&mPlayer);
    mSceneWidget->setGameItems(&mItems);

    mFrameTimer.setInterval(1000/FPS);
    mFrameTimer.setSingleShot(false);
    connect(&mFrameTimer, SIGNAL(timeout()), this, SLOT(updateFrame()));

    restart();
}

GameLogic::~GameLogic()
{
    for (AbstractItem * item : mItems)
    {
        delete item;
    }
}

void GameLogic::handleWallCollisions(double oldX, double oldY, double newX, double newY)
{
    // Calcul des index des cases pré et post déplacement
    int oldXi = std::floor(oldX);
    int oldYi = std::floor(oldY);
    int newXi = std::floor(newX);
    int newYi = std::floor(newY);

    // Booléen vrai si la nouvelle position est en dehors du labyrinthe
    bool oobNewPos = (newXi < 0 || newYi < 0 || newXi > MAZE_WIDTH - 1 || newYi > MAZE_HEIGHT - 1);

    // Si on est sorti des limites, on vérifie qu'il n'y a pas de mur (le cas échéant, il s'agit de la sortie)
    if (oobNewPos)
    {
        Cell& oldCell = mMaze.getCell(oldXi, oldYi);
        int diffx = newXi - oldXi;
        int diffy = newYi - oldYi;
        int direction = abs(diffx * (1 - diffx) + diffy * (2 + diffy));   //Trust me

        // Si on ne va pas en diagonale, et s'il n'y a pas de mur
        if ((diffx != 0 || diffy != 0) && !oldCell.isFrontier(Cell::Direction(direction)))
        {
            // On gagne le jeu
            win();
            return;
        }
        mPlayer.setPosition(oldX, oldY);
        return;
    }

    // Booléens de collision
    bool collisionE = (newXi > oldXi && mMaze.getCell(oldXi, oldYi).isFrontier(Cell::E));
    bool collisionW = (newXi < oldXi && mMaze.getCell(oldXi, oldYi).isFrontier(Cell::W));
    bool collisionN = (newYi < oldYi && mMaze.getCell(oldXi, oldYi).isFrontier(Cell::N));
    bool collisionS = (newYi > oldYi && mMaze.getCell(oldXi, oldYi).isFrontier(Cell::S));

    // Booléens de collisions "inversés" (calculés à partir de la nouvelle position)
    bool rCollisionE = (oldXi < newXi && mMaze.getCell(newXi, newYi).isFrontier(Cell::W));
    bool rCollisionW = (oldXi > newXi && mMaze.getCell(newXi, newYi).isFrontier(Cell::E));
    bool rCollisionN = (newYi >= 0 && oldYi > newYi && mMaze.getCell(newXi, newYi).isFrontier(Cell::S));
    bool rCollisionS = (mMaze.getCell(newXi, newYi).isFrontier(Cell::N));

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

void GameLogic::handleItemCollisions(double playerx, double playery)
{
    double xMin, xMax, yMin, yMax;

    for (auto iterItem = mItems.begin(); iterItem < mItems.end();)
    {
        AbstractItem * item = *iterItem;
        xMin = item->getPosX() - item->getRadius();
        xMax = item->getPosX() + item->getRadius();
        yMin = item->getPosY() - item->getRadius();
        yMax = item->getPosY() + item->getRadius();

        // Cas de collision
        if (playerx > xMin && playerx < xMax && playery > yMin && playery < yMax)
        {
            item->onCollision();
            iterItem = mItems.erase(iterItem);
        }
        else
        {
            ++iterItem;
        }
    }
}

void GameLogic::win()
{
    mFrameTimer.stop();

    QTime time = mTimerWidget->stop();
    QString finishMessage = QString("Vous êtes sortis du labyrinthe en %1:%2:%3")
            .arg(time.minute(), 2, 10, QChar('0'))
            .arg(time.second(), 2, 10, QChar('0'))
            .arg(time.msec(), 3, 10, QChar('0'));
    QMessageBox::information(nullptr, QString("Bravo !"), finishMessage);
}

void GameLogic::updateFrame()
{
    mSceneWidget->update();
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
            handleItemCollisions(mPlayer.getPosX(), mPlayer.getPosY());
            break;
        case Direction::BACKWARD:
            mPlayer.goBackward();
            handleWallCollisions(formerX, formerY, mPlayer.getPosX(), mPlayer.getPosY());
            handleItemCollisions(mPlayer.getPosX(), mPlayer.getPosY());
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

    if (direction == Direction::STOP)
    {
        mMapWidget->show();
    }
    else
    {
        mMapWidget->hide();
    }
}

void GameLogic::restart()
{
    mMaze.reinit();
    mMaze.generate();

    mPlayer.setPosition(0.5, 0.5);
    mPlayer.setAngle(0.0);

    mItems.clear();

    int goalXIdx = rand() % mMaze.getWidth();
    int goalYIdx = rand() % mMaze.getHeight();
    mItems.push_back(new GoalItem(&mMaze, mMapWidget, goalXIdx + 0.5, goalYIdx + 0.5));

    for (AbstractItem * item : mItems)
    {
        item->initGL();
    }

    mFrameTimer.start();

    mTimerWidget->restart();

    mMapWidget->show();
}

void GameLogic::changeOptions()
{
    mTimerWidget->pause();

    OptionsDialog dialog(mMaze.getWidth(), mMaze.getHeight());
    int dialogResponse = dialog.exec();

    if (dialogResponse == QDialog::Accepted)
    {
        mMaze.resize(dialog.getMazeWidth(), dialog.getMazeHeight());
        restart();
    }
    else
    {
        mTimerWidget->resume();
    }
}
