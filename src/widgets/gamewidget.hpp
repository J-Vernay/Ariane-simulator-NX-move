#ifndef GAMEWIDGET_HPP
#define GAMEWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include "glscenewidget.hpp"
#include "mazemapwidget.hpp"
#include "timerwidget.hpp"
#include "../gamelogic.hpp"

/**
 * @brief Widget principal du jeu
 */
class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);

    void resizeEvent(QResizeEvent *event) override;

    // Touches claviers : TEMPORAIRE
    void keyPressEvent(QKeyEvent *event) override;

private:
    // Widgets contenus
    GLSceneWidget mSceneWidget;
    MazeMapWidget mMapWidget;
    TimerWidget mTimerWidget;

    // Layout principal
    QVBoxLayout * mMainLayout;

    // Logique de jeu
    GameLogic mGameLogic;

};

#endif // GAMEWIDGET_HPP
