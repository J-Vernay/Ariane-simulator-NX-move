#include "mazemapwidget.hpp"
#include <QPainter>
#include <QDebug>

const double MAXOPACITY = 0.7;

MazeMapWidget::MazeMapWidget(QWidget *parent)
    : QWidget(parent), mOpacityEffect(this), mOpacityAnimation(this)
{
    // Gestion de l'effet d'opacité
    mOpacityEffect.setOpacity(MAXOPACITY);
    this->setGraphicsEffect(&mOpacityEffect);
    this->setWindowFlag(Qt::FramelessWindowHint);

    // Gestion de l'animation de fade-in / fade-out
    mOpacityAnimation.setTargetObject(&mOpacityEffect);
    mOpacityAnimation.setPropertyName("opacity");
    mOpacityAnimation.setDuration(500);
}

void MazeMapWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRectF bounds(0, 0, width(), height());
    painter.fillRect(bounds, Qt::white);
}

void MazeMapWidget::hide()
{
    if (mOpacityEffect.opacity() >= MAXOPACITY)
    {
        mOpacityAnimation.setEasingCurve(QEasingCurve::OutCurve);
        mOpacityAnimation.setStartValue(MAXOPACITY);
        mOpacityAnimation.setEndValue(0.0);
        mOpacityAnimation.start();
    }
}

void MazeMapWidget::show()
{
    if (mOpacityEffect.opacity() <= 0.0)
    {
        mOpacityAnimation.setEasingCurve(QEasingCurve::InCurve);
        mOpacityAnimation.setStartValue(0.0);
        mOpacityAnimation.setEndValue(MAXOPACITY);
        mOpacityAnimation.start();
    }
}
