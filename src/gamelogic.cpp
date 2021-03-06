#include "gamelogic.hpp"

#include <QMessageBox>

const int DEFAULT_MAZE_WIDTH = 10;
const int DEFAULT_MAZE_HEIGHT = 8;

const int FPS = 30;

const double COLLISION_MARGIN = 0.1;
const double COLLISION_ITEM_MARGIN = 0.33;

GameLogic::GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget, TimerWidget * timerWidget, QObject * parent)
    : QObject(parent), mSceneWidget(openGLSceneWidget), mMapWidget(miniMapWidget), mTimerWidget(timerWidget),
      mMaze(DEFAULT_MAZE_WIDTH, DEFAULT_MAZE_HEIGHT)
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
    // Rayon de la hitbox
    double hitbox = mPlayer.getHitboxRadius();

    // Calcul des index des cases pré et post déplacement
    int oldXi = std::floor(oldX);
    int oldYi = std::floor(oldY);
    int newXi = std::floor(newX);
    int newYi = std::floor(newY);

    // Booléen vrai si la nouvelle position est en dehors du labyrinthe
    bool oobNewPos = (newXi < 0 || newYi < 0 || newXi > mMaze.getWidth() - 1 || newYi > mMaze.getHeight() - 1);

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

    // Parties décimales de la nouvelle position (ex : 3.4 => 0.4)
    double newXd = newX - newXi;
    double newYd = newY - newYi;

    // Nouvelle position relative à l'indice de l'ancienne case
    double newXr = newX - oldXi;
    double newYr = newY - oldYi;

    // Collisions "classiques" avec un mur
    //   Note : on modifie directement newX et newY. Pour vérifier si newX ou newY a changé,
    //   on peut utiliser respectivement newXr + oldXi et newYr + oldYi, qui sont les anciennes
    //   valeurs de newX et newY
    Cell& oldCell = mMaze.getCell(oldXi, oldYi);
    if (oldCell.isFrontier(Cell::N))
    {
        newY = std::max(hitbox + COLLISION_MARGIN, newYr) + oldYi;
    }
    if (oldCell.isFrontier(Cell::S) && newY == (newYr + oldYi))
    {
        newY = std::min(1 - hitbox - COLLISION_MARGIN, newYr) + oldYi;
    }
    if (oldCell.isFrontier(Cell::W))
    {
        newX = std::max(hitbox + COLLISION_MARGIN, newXr) + oldXi;
    }
    if (oldCell.isFrontier(Cell::E) && newX == (newXr + oldXi))
    {
        newX = std::min(1 - hitbox - COLLISION_MARGIN, newXr) + oldXi;
    }
    mPlayer.setPosition(newX, newY);

    // On interdit de se retrouver dans un mur
    Cell& newCell = mMaze.getCell(newXi, newYi);
    if (newX == (newXr + oldXi) && newY == (newYr + oldYi) &&
               ((newCell.isFrontier(Cell::N) && newYd <= hitbox)
            || (newCell.isFrontier(Cell::S) && newYd >= (1 - hitbox))
            || (newCell.isFrontier(Cell::W) && newXd <= hitbox)
            || (newCell.isFrontier(Cell::E) && newXd >= (1 - hitbox))))
    {
        mPlayer.setPosition(oldX, oldY);
        return;
    }
}

void GameLogic::handleItemCollisions(double playerx, double playery)
{
    double xMin, xMax, yMin, yMax;

    for (auto iterItem = mItems.begin(); iterItem < mItems.end();)
    {
        AbstractItem * item = *iterItem;
        xMin = item->getPosX() - (1 + COLLISION_ITEM_MARGIN)*item->getRadius();
        xMax = item->getPosX() + (1 + COLLISION_ITEM_MARGIN)*item->getRadius();
        yMin = item->getPosY() - (1 + COLLISION_ITEM_MARGIN)*item->getRadius();
        yMax = item->getPosY() + (1 + COLLISION_ITEM_MARGIN)*item->getRadius();

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
    restart();
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

    // Création de tous les couples de coordonnées libres (autre que (0,0), qui est la position du joueur)
    struct CellCoord
    {
        int width = 0;
        int x = 0, y = 0;
        CellCoord & operator++()
        {
            if ((++x %= width) == 0) ++y;
            return *this;
        }
    };
    std::vector<CellCoord> freeCoords(mMaze.getWidth() * mMaze.getHeight() - 1);
    CellCoord initialCellCoord = ++CellCoord{mMaze.getWidth()};
    std::iota(freeCoords.begin(), freeCoords.end(), initialCellCoord);

    // On enlève la case de départ des coordonnées possibles
    freeCoords.erase(freeCoords.begin());

    //Mélange aléatoire des positions
    std::random_shuffle(freeCoords.begin(), freeCoords.end());

    // Génération de l'arrivée
    int goalXIdx = freeCoords.back().x;
    int goalYIdx = freeCoords.back().y;
    freeCoords.pop_back();
    mItems.push_back(new GoalItem(&mMaze, mMapWidget, goalXIdx + 0.5, goalYIdx + 0.5));

    if (mIsBonusTimeEnabled)
    {
        int bonustimeXIdx = freeCoords.back().x;
        int bonustimeYIdx = freeCoords.back().y;
        freeCoords.pop_back();
        mItems.push_back(new BonusTimeItem(mTimerWidget, bonustimeXIdx + 0.5, bonustimeYIdx + 0.5));
    }

    if (mIsPenaltyTimeEnabled)
    {
        int penaltytimeXIdx = freeCoords.back().x;
        int penaltytimeYIdx = freeCoords.back().y;
        freeCoords.pop_back();
        mItems.push_back(new PenaltyTimeItem(mTimerWidget, penaltytimeXIdx + 0.5, penaltytimeYIdx + 0.5));
    }

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

    OptionsDialog dialog(mMaze.getWidth(), mMaze.getHeight(), mIsBonusTimeEnabled, mIsPenaltyTimeEnabled);
    int dialogResponse = dialog.exec();

    if (dialogResponse == QDialog::Accepted)
    {
        mMaze.resize(dialog.getMazeWidth(), dialog.getMazeHeight());
        mIsBonusTimeEnabled = dialog.getIsBonusTimeEnabled();
        mIsPenaltyTimeEnabled = dialog.getIsPenaltyTimeEnabled();
        restart();
    }
    else
    {
        mTimerWidget->resume();
    }
}
