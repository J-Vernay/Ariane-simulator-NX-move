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
    double mPosx, mPosy;            // Position de l'objet, en coordonnées logiques (1.0 => une case du labyrinthe)
    double mRadius;                 // Rayon de la sphere représentant l'objet
    double mAngle = 0.0;            // Angle de rotation de la sphere représentant l'objet
    bool mIsBad;                    // VRAI si l'objet est un malus
    QElapsedTimer mElapsedTimer;    // Temps écoulé depuis la création de l'objet, utile pour l'animation

    // Quadrique de la sphere. Appelle automatiquement gluDeleteQuadric() en cas de destruction du pointeur
    std::unique_ptr<GLUquadric, void(*)(GLUquadric *)> mQuadric;
    GLuint mTextureId;              // Identifiant de la texture de la sphere
    QString mTexturePath;           // Chemin vers la ressource de la texture de la sphere

public:
    // Constructeur
    AbstractItem(double xpos, double ypos, QString texturePath = QString(""), double radius = 0.15, bool isBad = false);

    // Destructeur (virtuel car polymorphisme)
    virtual ~AbstractItem(){};

    // Initialisation des éléments OpenGL
    void initGL();

    // Fonction d'affichage de l'objet en OpenGL
    void displayGL(const double xStep, const double yStep, const double zStep);

    //Getters
    double getPosX() { return mPosx; };
    double getPosY() { return mPosy; };
    double getRadius() { return mRadius; };

    // Méthode virtuelle pure, à redéfinir dans la classe dérivée.
    //   Définie les actions à réalisée lorsque le joueur récupère l'objet
    virtual void onCollision() = 0;
};

#endif // ABSTRACTITEM_HPP
