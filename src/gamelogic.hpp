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

/**
 * @brief Classe principale de la logique de jeu
 */
class GameLogic : public QObject
{
    Q_OBJECT

    GLSceneWidget * mSceneWidget;       // Scene 3D OpenGL
    MazeMapWidget * mMapWidget;         // Plan du labyrinthe
    TimerWidget * mTimerWidget;         // Chronomètre

    Maze mMaze;                         // Labyrinthe
    Player mPlayer;                     // Joueur

    std::vector<AbstractItem *> mItems; // Objets collectibles

    QTimer mFrameTimer;                 // Timer de rafraichissement => lié au slot updateFrame()

    bool mIsBonusTimeEnabled = true, mIsPenaltyTimeEnabled = true;  // Activation des bonus/malus de temps

public:
    // Constructeur
    GameLogic(GLSceneWidget * openGLSceneWidget, MazeMapWidget * miniMapWidget, TimerWidget * timerWidget, QObject * parent = nullptr);
    
    //Destructeur
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

    // Fonction de victoire, à appeler si le joueur sort du labyrinthe
    void win();

private slots:
    // Slot de rafraichissement, appelé periodiquement. En particulier, met à jour la scene OpenGL
    void updateFrame();

    // Slot permettant de redémarrer une partie
    void restart();

public slots:
    // Slot permettant de changer les options du jeu
    void changeOptions();
};

#endif // GAMELOGIC_HPP
