#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include "widgets/glscenewidget.hpp"
#include "widgets/mazemapwidget.hpp"
#include "widgets/timerwidget.hpp"
#include "widgets/optionsdialog.hpp"
#include "gameobjects/maze.h"
#include "gameobjects/player.hpp"
#include "gameobjects/abstractitem.hpp"
#include "gameobjects/goalitem.hpp"
#include "gameobjects/bonustimeitem.hpp"
#include "gameobjects/penaltytimeitem.hpp"

#include <QTimer>
#include <QTime>
#include <algorithm>

/**
 * @brief Classe principale de la logique de jeu
 */
class GameLogic : public QObject
{
    Q_OBJECT

    GLSceneWidget * mSceneWidget;
    MazeMapWidget * mMapWidget;
    TimerWidget * mTimerWidget;

    Maze mMaze;
    Player mPlayer;

    std::vector<AbstractItem *> mItems;

    QTimer mFrameTimer;

    bool mIsBonusTimeEnabled = true, mIsPenaltyTimeEnabled = true;

public:
    GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget, TimerWidget * timerWidget, QObject * parent = nullptr);
    ~GameLogic();

    // Fonction de déplacement du joueur, dans une direction définie par l'enum Direction.
    enum Direction{FORWARD, BACKWARD, LEFT, RIGHT, STOP};
    void movePlayer(Direction direction);

private:
    // Appeler après un déplacement logique. Cette méthode va corriger le déplacement du
    //      joueur ssi ce dernier est entré en collision avec un mur
    void handleWallCollisions(double oldX, double oldY, double newX, double newY);

    // Vérifie les collisions avec un éventuel item, et appelle la méthode correspondante le cas échéant
    void handleItemCollisions(double playerx, double playery);

    void win();

private slots:
    void updateFrame();
    void restart();

public slots:
    void changeOptions();
};

#endif // GAMELOGIC_HPP
