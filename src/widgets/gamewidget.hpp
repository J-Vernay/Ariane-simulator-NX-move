#ifndef GAMEWIDGET_HPP
#define GAMEWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include "glscenewidget.hpp"
#include "mazemapwidget.hpp"
#include "timerwidget.hpp"
#include "camerawidget.hpp"
#include "../imageinput.hpp"
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

    // Thread et widget pour la camera
    CameraThread mCameraThread;
    CameraWidget mCameraWidget;
    // timer pour vérifier la direction détectée périodiquement
    QTimer* mCameraTimer;
    GameLogic::Direction mLastDir = GameLogic::STOP;

    // Layout principal
    QVBoxLayout * mMainLayout;

    //Layout horizontal des boutons
    QWidget * mButtonsWidget;
    QHBoxLayout * mButtonsLayout;

    // Logique de jeu
    GameLogic mGameLogic;

private slots:
    void getCameraDirection();
    void quit();

};

#endif // GAMEWIDGET_HPP
