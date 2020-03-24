#include "gamewidget.hpp"

#include <QPushButton>
#include <QKeyEvent>

// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;
const double MINIMAP_SCALE = 0.2;


GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent), mSceneWidget(parent), mMapWidget(&mSceneWidget), mGameLogic(&mSceneWidget, &mMapWidget)
{
    resize(WIN_WIDTH, WIN_HEIGHT);

    mMainLayout = new QVBoxLayout(this);
    mMainLayout->addWidget(&mSceneWidget);

    QPushButton * hideButton = new QPushButton("HIDE", this);
    mMainLayout->addWidget(hideButton);
    connect(hideButton, SIGNAL(pressed()), &mMapWidget, SLOT(hide()));

    QPushButton * showButton = new QPushButton("SHOW", this);
    mMainLayout->addWidget(showButton);
    connect(showButton, SIGNAL(pressed()), &mMapWidget, SLOT(show()));

    this->setLayout(mMainLayout);

    mSceneWidget.setFocusPolicy(Qt::StrongFocus);
}

void GameWidget::resizeEvent(QResizeEvent *event)
{
    mMapWidget.setGeometry(0, 0, width() * MINIMAP_SCALE, height() * MINIMAP_SCALE);
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
    }
}
