#include "mazemapwidget.hpp"
#include <QPainter>

MazeMapWidget::MazeMapWidget(QWidget *parent) : QWidget(parent), mOpacityEffect(this)
{
    mOpacityEffect.setOpacity(0.5);
    this->setGraphicsEffect(&mOpacityEffect);
}

void MazeMapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRectF bounds(0, 0, width(), height());
    painter.fillRect(bounds, Qt::white);
}
