#include "gamewidget.hpp"

#include <QPushButton>
#include <QKeyEvent>

// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;
const double MINIMAP_SCALE = 0.2;


GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent), mSceneWidget(parent), mMapWidget(&mSceneWidget), mTimerWidget(&mSceneWidget),
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
}

void GameWidget::resizeEvent(QResizeEvent *event)
{
    mMapWidget.setGeometry(0, 0, width() * MINIMAP_SCALE, height() * MINIMAP_SCALE);

    int timerposx = (mSceneWidget.width() - mTimerWidget.width()) / 2;
    mTimerWidget.move(timerposx, 0);
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
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
}

void GameWidget::quit()
{
    this->close();
}
