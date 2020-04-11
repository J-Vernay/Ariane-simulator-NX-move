#ifndef GLSCENEWIDGET_HPP
#define GLSCENEWIDGET_HPP

#include <QOpenGLWidget>
#include "../gameobjects/maze.h"
#include "../gameobjects/cell.h"
#include "../gameobjects/player.hpp"
#include "../gameobjects/abstractitem.hpp"

/**
 * @brief Classe d'affichage de la scene 3D OpenGL
 */
class GLSceneWidget : public QOpenGLWidget
{
    Q_OBJECT

    Maze * mMaze;           // Labyrinthe
    Player * mPlayer;       // Joueur

    std::vector<AbstractItem *> * mGameItems;   // Objets collectibles

public:
    // Constructeur
    explicit GLSceneWidget(QWidget *parent = nullptr);

    // Asssociation avec le labyrinthe
    void setMaze(Maze * maze) { mMaze = maze; };

    // Association avec le joueur
    void setPlayer(Player * player) { mPlayer = player; };

    // Association avec la collection d'items
    void setGameItems(std::vector<AbstractItem *> * gameItems) { mGameItems = gameItems; };

protected:

    // Fonction d'initialisation d'OpenGL
    void initializeGL();

    // Fonction de redimensionnement du widget OpenGL
    void resizeGL(int width, int height);

    // Fonction d'affichage OpenGL
    void paintGL();

    // Mise à jour de la position de la caméra
    void updateView();

    // Affichage du labyrinthe
    void displayWorld();
};

#endif // GLSCENEWIDGET_HPP
