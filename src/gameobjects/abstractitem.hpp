#ifndef ABSTRACTITEM_HPP
#define ABSTRACTITEM_HPP

#include <GL/glu.h>
#include <memory>
#include <QElapsedTimer>
#include <QString>

/**
 * @brief Classe abstraite représentant un objet à récupérer dans le labyrinthe
 */
class AbstractItem
{
protected:
    double mPosx, mPosy;
    double mRadius;
    double mAngle = 0.0;
    bool mIsBad;
    QElapsedTimer mElapsedTimer;

    std::unique_ptr<GLUquadric, void(*)(GLUquadric *)> mQuadric;
    GLuint mTextureId;
    QString mTexturePath;

public:
    AbstractItem(double xpos, double ypos, double radius = 0.1, QString texturePath = QString(""), bool isBad = false);
    virtual ~AbstractItem(){};

    void initGL();
    void displayGL(const double xStep, const double yStep, const double zStep);

    //Getters
    double getPosX() { return mPosx; };
    double getPosY() { return mPosy; };
    double getRadius() { return mRadius; };

    virtual void onCollision() = 0;
};

#endif // ABSTRACTITEM_HPP
