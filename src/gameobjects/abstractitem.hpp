#ifndef ABSTRACTITEM_HPP
#define ABSTRACTITEM_HPP

#include <GL/glu.h>
#include <memory>
#include <QElapsedTimer>
#include <QString>


class AbstractItem
{
    double mPosx, mPosy;
    double mRadius;
    double mAngle = 0.0;
    QElapsedTimer mElapsedTimer;

    std::unique_ptr<GLUquadric, void(*)(GLUquadric *)> mQuadric;
    GLuint mTextureId;
    QString mTexturePath;

public:
    AbstractItem(double xpos, double ypos, double radius = 0.1, QString texturePath = QString(""));

    void initGL();
    void displayGL(const double xStep, const double yStep, const double zStep);
};

#endif // ABSTRACTITEM_HPP
