#include "abstractitem.hpp"

AbstractItem::AbstractItem(double xpos, double ypos, double radius)
    : mPosx(xpos), mPosy(ypos), mRadius(radius), mQuadric(gluNewQuadric(), gluDeleteQuadric)
{
    gluQuadricDrawStyle(mQuadric.get(), GLU_FILL);
}

void AbstractItem::displayGL(const double xStep, const double yStep, const double zStep) const
{
    glPushMatrix();
    glTranslated(mPosx * xStep, mPosy * yStep, 0.5 * zStep);
    gluSphere(mQuadric.get(), mRadius * xStep, 20, 20);
    glPopMatrix();
}
