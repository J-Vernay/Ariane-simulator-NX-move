#ifndef GAMEWIDGET_HPP
#define GAMEWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include "glscenewidget.hpp"

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);

private:
    GLSceneWidget mSceneWidget;
    QVBoxLayout * mainLayout;
};

#endif // GAMEWIDGET_HPP
