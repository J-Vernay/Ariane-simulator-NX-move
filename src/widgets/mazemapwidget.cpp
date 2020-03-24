#include "mazemapwidget.hpp"
#include <QPainter>
#include <cmath>
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

    // Background
    QRectF bounds(0, 0, width(), height());
    painter.fillRect(bounds, Qt::white);

    // Définition de constantes
    const int MARGIN = 10;
    const int BEGIN = MARGIN;
    const int Y_STEP = (height() - 2 * MARGIN) / mMaze->getHeight();
    const int X_STEP = (width() - 2 * MARGIN) / mMaze->getWidth();
    const int PLAYER_RADIUS = 5;

    // Dessin du labyrinthe
    if (mMaze != nullptr)
    {
        // Configuration du stylo
        painter.setPen(Qt::black);

        // Parcours de la grille
        auto grid = mMaze->getGrid();
        int currx, curry;
        for(unsigned int y = 0; y < grid.size(); ++y)
        {
            for(unsigned int x = 0; x < grid[y].size(); ++x)
            {
                Cell cell = grid[y][x];

                currx = BEGIN + x * X_STEP;
                curry = BEGIN + y * Y_STEP;

                if (cell.isFrontier(Cell::N))
                {
                    painter.drawLine(currx, curry, currx + X_STEP, curry);
                }
                if (cell.isFrontier(Cell::S))
                {
                    painter.drawLine(currx, curry + Y_STEP, currx + X_STEP, curry + Y_STEP);
                }
                if (cell.isFrontier(Cell::W))
                {
                    painter.drawLine(currx, curry, currx, curry + Y_STEP);
                }
                if (cell.isFrontier(Cell::E))
                {
                    painter.drawLine(currx + X_STEP, curry, currx + X_STEP, curry + Y_STEP);
                }
            }
        }
    }

    // Dessin du joueur
    if(mPlayer != nullptr)
    {
        // Selection du stylo
        painter.setBrush(Qt::green);

        // Récupération des coordonnées, et application à l'échelle de la minimap
        int xPosition = BEGIN + mPlayer->getPosX() * X_STEP;
        int yPosition = BEGIN + mPlayer->getPosY() * Y_STEP;
        painter.drawEllipse(QPoint(xPosition, yPosition), PLAYER_RADIUS, PLAYER_RADIUS);

        double angle = mPlayer->getAngle();
        painter.setPen(Qt::darkGreen);
        painter.drawLine(xPosition, yPosition, xPosition + std::cos(angle) * X_STEP * 0.5, yPosition + std::sin(angle) * Y_STEP * 0.5);
    }
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
