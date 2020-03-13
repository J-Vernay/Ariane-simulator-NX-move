#include "gamewidget.hpp"

// Declarations des constantes
const unsigned int WIN_WIDTH  = 1600;
const unsigned int WIN_HEIGHT = 900;

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent), mSceneWidget(parent)
{
    resize(WIN_WIDTH, WIN_HEIGHT);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(&mSceneWidget);
    this->setLayout(mainLayout);
}
