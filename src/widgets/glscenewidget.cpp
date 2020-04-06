#include "glscenewidget.hpp"
#include <QApplication>
#include <GL/glu.h>
#include <QDesktopWidget>
#include <cmath>
#include <QDebug>

const GLdouble X_STEP = 10;
const GLdouble Y_STEP = -X_STEP;
const GLdouble WALL_HEIGHT = 5;

GLSceneWidget::GLSceneWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    //move(QApplication::desktop()->rect().center() - rect().center());
}

void GLSceneWidget::updateView()
{
    if (mPlayer == nullptr) return;

    // La position du joueur est gérée directement via la position de la caméra.
    double xpos = mPlayer->getPosX() * X_STEP;
    double ypos = mPlayer->getPosY() * Y_STEP;
    double centerx = xpos + std::cos(mPlayer->getAngle());
    double centery = ypos - std::sin(mPlayer->getAngle());

    gluLookAt(xpos, ypos, WALL_HEIGHT / 2.0,
              centerx, centery, WALL_HEIGHT / 2.0,
              0.0, 0.0, 1.0);
}

void GLSceneWidget::displayWorld()
{
    GLfloat gray[] = { 0.5, 0.5, 0.5, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, gray);

    glBegin(GL_QUADS);

    // Dessin des murs
    for(int y = 0; y < mMaze->getHeight(); ++y)
    {
        for(int x = 0; x < mMaze->getWidth(); ++x)
        {
            Cell cell = mMaze->getCell(x, y);

            if(cell.isFrontier(Cell::E))
            {
                glNormal3d(-X_STEP, 0.0, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP + Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP + Y_STEP, WALL_HEIGHT);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP, WALL_HEIGHT);
            }
            if(cell.isFrontier(Cell::W))
            {
                glNormal3d(X_STEP, 0.0, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP + Y_STEP, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP + Y_STEP, WALL_HEIGHT);
                glVertex3d(x * X_STEP, y * Y_STEP, WALL_HEIGHT);
            }
            if(cell.isFrontier(Cell::N))
            {
                glNormal3d(0.0, Y_STEP, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP, WALL_HEIGHT);
                glVertex3d(x * X_STEP, y * Y_STEP, WALL_HEIGHT);
            }
            if(cell.isFrontier(Cell::S))
            {
                glNormal3d(0.0, -Y_STEP, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP + Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP + Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP + Y_STEP, WALL_HEIGHT);
                glVertex3d(x * X_STEP, y * Y_STEP + Y_STEP, WALL_HEIGHT);
            }

        }
    }

    // Dessin du sol (même en dehors du labyrinthe)
    GLfloat darkgreen[] = {0.2, 0.4, 0.2, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, darkgreen);
    glNormal3d(0.0, 0.0, 1.0);
    glVertex3d(-10 * X_STEP, -10 * Y_STEP, 0.0);
    glVertex3d((10 + mMaze->getWidth()) * X_STEP, -10 * Y_STEP, 0.0);
    glVertex3d((10 + mMaze->getWidth()) * X_STEP, (10 + mMaze->getHeight()) * Y_STEP, 0.0);
    glVertex3d(-10 * X_STEP, (10 + mMaze->getHeight()) * Y_STEP, 0.0);

    // Dessin du plafond, en gardant le materiau du sol
    GLfloat darkgray[] = {0.2, 0.2, 0.2, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, darkgray);
    glNormal3d(0.0, 0.0, -1.0);
    glVertex3d(0.0, 0.0, WALL_HEIGHT);
    glVertex3d(mMaze->getWidth() * X_STEP, 0.0, WALL_HEIGHT);
    glVertex3d(mMaze->getWidth() * X_STEP, mMaze->getHeight() * Y_STEP, WALL_HEIGHT);
    glVertex3d(0.0, mMaze->getHeight() * Y_STEP, WALL_HEIGHT);

    glEnd();
}

void GLSceneWidget::initializeGL()
{
    // Activation des textures
    glEnable(GL_TEXTURE_2D);

    // Reglage de la couleur de fond (bleu ciel)
    glClearColor(0.53,0.81,0.92, 1.0);

    // Activation du z-buffer
    glEnable(GL_DEPTH_TEST);

    // Règlage de la lumière ambiante
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);

    for (AbstractItem * item : *mGameItems)
    {
        item->initGL();
    }
}

void GLSceneWidget::resizeGL(int width, int height)
{
    // Definition du viewport (zone d'affichage)
    float zoom = std::min(width/16., height/9.);
    float w = zoom * 16, h = zoom * 9;
    glViewport((width-w)/2, (height-h)/2, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Reglage de la perspective
    gluPerspective(70.0, 16.0/9.0, 0.1, std::max(mMaze->getWidth(), mMaze->getHeight()) * X_STEP);

}

void GLSceneWidget::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Reset des tampons
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mMaze == nullptr) return;

    glPushMatrix();

    // Déplacement de la caméra
    updateView();

    // Affichage du monde
    displayWorld();

    // Affichage des items
    for (AbstractItem * item : *mGameItems)
    {
        item->displayGL(X_STEP, Y_STEP, WALL_HEIGHT);
    }

    glPopMatrix();
}
