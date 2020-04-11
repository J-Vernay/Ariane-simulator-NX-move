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
    // Constructeur
    explicit GameWidget(QWidget *parent = nullptr);

    // Fonction appelée lorsque le widget est redimmensionné. Met à jour la position et la taille 
    //   des widgets
    void resizeEvent(QResizeEvent *event) override;

private:

    // Widgets contenus
    GLSceneWidget mSceneWidget;
    MazeMapWidget mMapWidget;
    TimerWidget mTimerWidget;

    // Thread et widget pour la camera
    CameraThread mCameraThread;
    CameraWidget mCameraWidget;
    // Timer pour vérifier la direction détectée périodiquement
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
    // Slot à appeler afin de connaitre la direction actuellement détectée par la caméra
    void getCameraDirection();

    // Slot à appeler afin de quitter l'application
    void quit();

};

#endif // GAMEWIDGET_HPP
