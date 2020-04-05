#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <cmath>

/**
 * @brief Classe de gestion du joueur, indépendemment de l'affichage graphique ou du labyrinthe
 */
class Player
{
    double mPosX, mPosY;                        // Position logique : une case du labyrinthe constitue 1 unité
    double mAngle;                              // Orientation
    const double mWalkSpeed = 0.2;              // Vitesse de déplacement du joueur
    const double mRotateSpeed = M_PI / 30.0;    // Vitesse de rotation du joueur

public:
    Player(double posx = 0.5, double poxy = 0.5, double angle = 0.0);

    // Mutateur de la position brute
    void setPosition(double posx, double posy) { mPosX = posx; mPosY = posy; };

    // Mutateur de l'angle brut
    void setAngle(double angle) { mAngle = angle; };

    // Getters
    double getPosX() { return mPosX; };
    double getPosY() { return mPosY; };
    double getAngle() { return mAngle; };

    // Fonctions de déplacement
    void goForward();
    void goBackward();
    void turnLeft();
    void turnRight();
};

#endif // PLAYER_HPP
