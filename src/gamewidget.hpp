#ifndef GAMEWIDGET_HPP
#define GAMEWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include "glscenewidget.hpp"
#include "mazemapwidget.hpp"

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);

private:
    // Widgets
    GLSceneWidget mSceneWidget;
    MazeMapWidget mMapWidget;

    QVBoxLayout * mainLayout;

};

#endif // GAMEWIDGET_HPP
