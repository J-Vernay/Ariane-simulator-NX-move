#include "abstractitem.hpp"

#include <cmath>
#include <QGLWidget>
#include <GL/glu.h>
#include <QDebug>

const double Z_CENTER = 0.5;
const double Z_VARIATION = 0.1;
const double Z_SPEED = 1/500.0;
const double ANGLE_SPEED = 0.7;

AbstractItem::AbstractItem(double xpos, double ypos, QString texturePath, double radius, bool isBad)
    : mPosx(xpos), mPosy(ypos), mRadius(radius), mIsBad(isBad),
      mQuadric(gluNewQuadric(), gluDeleteQuadric), mTexturePath(texturePath)
{
    mElapsedTimer.start();
}

void AbstractItem::initGL()
{
    gluQuadricDrawStyle(mQuadric.get(), GLU_FILL);

    // Mise en place de la texture
    if (!mTexturePath.isEmpty())
    {
        QImage textureImage = QGLWidget::convertToGLFormat(QImage(mTexturePath));

        glGenTextures(1, &mTextureId);

        glBindTexture(GL_TEXTURE_2D, mTextureId);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     4,
                     textureImage.width(), textureImage.height(),
                     0,
                     GL_RGBA, GL_UNSIGNED_BYTE,
                     textureImage.bits());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gluQuadricTexture(mQuadric.get(), GL_TRUE);

    }
}

void AbstractItem::displayGL(const double xStep, const double yStep, const double zStep)
{
    glPushMatrix();

    // Calcul de la position Z de l'item
    double zpos = Z_CENTER + Z_VARIATION * std::sin(mElapsedTimer.elapsed() * Z_SPEED);

    // Affectation de la texture
    glBindTexture(GL_TEXTURE_2D, mTextureId);

    // Transformations du repère
    glTranslated(mPosx * xStep, mPosy * yStep, zpos * zStep);
    glRotated(mAngle, 0, 0, 1);

    // Mis en place de la lumière émise (bleu si bonus, rouge si malus)
    float emittedLight[] = {0.5, 0.5, 0.9, 1.0};
    if (mIsBad)
    {
        emittedLight[0] = 0.9;
        emittedLight[2] = 0.5;
    }
    glMaterialfv(GL_FRONT, GL_EMISSION, emittedLight);

    // Dessin de la sphere
    gluSphere(mQuadric.get(), mRadius * xStep, 20, 20);

    // Annulation de la lumière émise pour les prochains objets dessinés
    float noLight[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, noLight);

    // Actualisation de l'angle de l'item
    mAngle += ANGLE_SPEED;

    glPopMatrix();
}
