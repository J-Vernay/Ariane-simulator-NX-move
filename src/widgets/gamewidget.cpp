#include "gamewidget.hpp"

#include <QPushButton>
#include <QKeyEvent>
#include <QDebug>

// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;
const double MINIMAP_SCALE = 0.2;
const double CAMERA_SCALE = 0.3;

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent), mSceneWidget(parent), mMapWidget(&mSceneWidget), mTimerWidget(&mSceneWidget),
    mCameraThread(0), mCameraWidget(mCameraThread, &mSceneWidget),
    mGameLogic(&mSceneWidget, &mMapWidget, &mTimerWidget)
{
    resize(WIN_WIDTH, WIN_HEIGHT);

    mMainLayout = new QVBoxLayout(this);
    this->setLayout(mMainLayout);
    mMainLayout->addWidget(&mSceneWidget, 1);

    mButtonsWidget = new QWidget(this);
    mButtonsLayout = new QHBoxLayout(mButtonsWidget);
    mButtonsWidget->setLayout(mButtonsLayout);
    mMainLayout->addWidget(mButtonsWidget);

    mButtonsLayout->addStretch();

    QPushButton * optionsButton = new QPushButton("&Options", this);
    mButtonsLayout->addWidget(optionsButton);
    connect(optionsButton, SIGNAL(pressed()), &mGameLogic, SLOT(changeOptions()));

    QPushButton * restartButton = new QPushButton("&Recommencer", this);
    mButtonsLayout->addWidget(restartButton);
    connect(restartButton, SIGNAL(pressed()), &mGameLogic, SLOT(restart()));

    QPushButton * exitButton = new QPushButton("&Quitter", this);
    mButtonsLayout->addWidget(exitButton);
    connect(exitButton, SIGNAL(pressed()), this, SLOT(quit()));

    mSceneWidget.setFocusPolicy(Qt::StrongFocus);

    mCameraTimer = new QTimer(this);
    mCameraTimer->setInterval(100);
    connect(mCameraTimer, SIGNAL(timeout()), this, SLOT(getCameraDirection()));
    mCameraTimer->start();
}

void GameWidget::resizeEvent(QResizeEvent *event)
{
    mMapWidget.setGeometry(0, 0, width() * MINIMAP_SCALE, height() * MINIMAP_SCALE);

    int timerposx = (mSceneWidget.width() - mTimerWidget.width()) / 2;
    mTimerWidget.move(timerposx, 0);

    double cameraRatio = (double)(mCameraThread.getCameraFrameWidth()) / mCameraThread.getCameraFrameHeight();
    int cameraWidth = mSceneWidget.width() * CAMERA_SCALE, cameraHeight = cameraWidth / cameraRatio;
    int cameraposx = (mSceneWidget.width() - cameraWidth);
    mCameraWidget.setGeometry(cameraposx, 0, cameraWidth,cameraHeight);

    update();
}

void GameWidget::keyPressEvent(QKeyEvent *) {}

void GameWidget::getCameraDirection()
{
    switch (mCameraThread.getDirection()) {
    case Direction::UP:
        mLastDir = GameLogic::Direction::FORWARD;
        break;
    case Direction::DOWN:
        mLastDir = GameLogic::Direction::BACKWARD;
        break;
    case Direction::LEFT:
        mLastDir = GameLogic::Direction::LEFT;
        break;
    case Direction::RIGHT:
        mLastDir = GameLogic::Direction::RIGHT;
        break;
    case Direction::NONE:
        mLastDir = GameLogic::Direction::STOP;
        break;
    default:
        // dans le cas où Direction::UNKNOWN, on garde la direction d'avant
        break;
    }
    mGameLogic.movePlayer(mLastDir);
#if 0
    switch(event->key())
    {
        case Qt::Key_Up:
            mGameLogic.movePlayer(GameLogic::Direction::FORWARD);
            break;
        case Qt::Key_Down:
            mGameLogic.movePlayer(GameLogic::Direction::BACKWARD);
            break;
        case Qt::Key_Left:
            mGameLogic.movePlayer(GameLogic::Direction::LEFT);
            break;
        case Qt::Key_Right:
            mGameLogic.movePlayer(GameLogic::Direction::RIGHT);
            break;
        case Qt::Key_Return:
            mGameLogic.movePlayer(GameLogic::Direction::STOP);
            break;
    }
#endif
}

void GameWidget::quit()
{
    this->close();
}
