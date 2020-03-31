#ifndef ABSTRACTITEM_HPP
#define ABSTRACTITEM_HPP

#include <GL/glu.h>
#include <memory>
#include <QElapsedTimer>


class AbstractItem
{
    double mPosx, mPosy;
    double mRadius;
    QElapsedTimer mElapsedTimer;

    std::unique_ptr<GLUquadric, void(*)(GLUquadric *)> mQuadric;

public:
    AbstractItem(double xpos, double ypos, double radius = 0.1);

    void displayGL(const double xStep, const double yStep, const double zStep) const;
};

#endif // ABSTRACTITEM_HPP
