#ifndef GAMELOGIC_HPP
#define GAMELOGIC_HPP

#include "widgets/glscenewidget.hpp"
#include "widgets/mazemapwidget.hpp"
#include "widgets/timerwidget.hpp"
#include "gameobjects/maze.h"
#include "gameobjects/player.hpp"
#include "gameobjects/abstractitem.hpp"

#include <QTimer>

/**
 * @brief Classe principale de la logique de jeu
 */
class GameLogic : QObject
{
    Q_OBJECT

    GLSceneWidget * mSceneWidget;
    MazeMapWidget * mMapWidget;
    TimerWidget * mTimerWidget;

    Maze mMaze;
    Player mPlayer;

    std::vector<AbstractItem *> mItems;

    QTimer mFrameTimer;

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

private slots:
    void updateFrame();
};

#endif // GAMELOGIC_HPP
