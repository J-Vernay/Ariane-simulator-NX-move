#include "abstractitem.hpp"

#include <cmath>

const double Z_CENTER = 0.3;
const double Z_VARIATION = 0.1;
const double Z_SPEED = 1/500.0;

AbstractItem::AbstractItem(double xpos, double ypos, double radius)
    : mPosx(xpos), mPosy(ypos), mRadius(radius), mQuadric(gluNewQuadric(), gluDeleteQuadric)
{
    gluQuadricDrawStyle(mQuadric.get(), GLU_FILL);

    mElapsedTimer.start();
}

void AbstractItem::displayGL(const double xStep, const double yStep, const double zStep) const
{
    glPushMatrix();

    double zpos = Z_CENTER + Z_VARIATION * std::sin(mElapsedTimer.elapsed() * Z_SPEED);

    glTranslated(mPosx * xStep, mPosy * yStep, zpos * zStep);
    gluSphere(mQuadric.get(), mRadius * xStep, 20, 20);


    glPopMatrix();
}
