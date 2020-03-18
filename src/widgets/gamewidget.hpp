#ifndef GAMEWIDGET_HPP
#define GAMEWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include "glscenewidget.hpp"
#include "mazemapwidget.hpp"

/**
 * @brief Widget principal du jeu
 */
class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);

private:
    // Widgets contenus
    GLSceneWidget mSceneWidget;
    MazeMapWidget mMapWidget;

    // Layout principal
    QVBoxLayout * mainLayout;
};

#endif // GAMEWIDGET_HPP
