#ifndef MAZEMAPWIDGET_HPP
#define MAZEMAPWIDGET_HPP

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QTimer>

#include "../models/maze.h"

/**
 * @brief Classe d'affichage de la minimap
 */
class MazeMapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MazeMapWidget(QWidget *parent = nullptr);

    // Fonction de dessin
    void paintEvent(QPaintEvent *event) override;

    // Fonction pour désigner le labyrinthe
    void setMaze(Maze * maze) { mMaze = maze; };

private:
    // Effet d'affichage en transparence
    QGraphicsOpacityEffect mOpacityEffect;

    // Animation de fade-in / fade-out
    QPropertyAnimation mOpacityAnimation;

    // Labyrinthe
    Maze * mMaze = nullptr;


public slots:
    // Slots pour afficher / cacher la minimap
    void hide();
    void show();

};

#endif // MAZEMAPWIDGET_HPP
