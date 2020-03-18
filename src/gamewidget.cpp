#include "gamewidget.hpp"

#include <QPushButton>

// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent), mSceneWidget(parent), mMapWidget(&mSceneWidget)
{
    resize(WIN_WIDTH, WIN_HEIGHT);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(&mSceneWidget);

    QPushButton * hideButton = new QPushButton("HIDE", this);
    mainLayout->addWidget(hideButton);
    connect(hideButton, SIGNAL(pressed()), &mMapWidget, SLOT(hide()));

    QPushButton * showButton = new QPushButton("SHOW", this);
    mainLayout->addWidget(showButton);
    connect(showButton, SIGNAL(pressed()), &mMapWidget, SLOT(show()));

    this->setLayout(mainLayout);

    mMapWidget.setGeometry(0, 0, 300, 200);
}
