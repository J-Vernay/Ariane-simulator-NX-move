#include "glscenewidget.hpp"
#include <QApplication>
#include <GL/glu.h>
#include <QDesktopWidget>
#include <cmath>
#include <QDebug>

const GLdouble X_STEP = 10;
const GLdouble Y_STEP = -X_STEP;
const GLdouble WALL_HEIGHT = 5;
const GLdouble WALL_WIDTH = 0.1;

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

void GLSceneWidget::initializeGL()
{
    // Reglage de la couleur de fond
    glColor3ub(0,0,0);

    // Activation du z-buffer
    glEnable(GL_DEPTH_TEST);

    // Règlage de la lumière ambiante
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);

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
    gluPerspective(70.0, 16.0/9.0, 0.1, 10 * X_STEP);

}

void GLSceneWidget::paintGL()
{
    qDebug() << "PAINT";

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Reset des tampons
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(mMaze == nullptr) return;

    glPushMatrix();

    // Déplacement de la caméra
    updateView();

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
                glNormal3d(-1.0, 0.0, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP + Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP + Y_STEP, WALL_HEIGHT);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP, WALL_HEIGHT);
            }
            if(cell.isFrontier(Cell::W))
            {
                glNormal3d(1.0, 0.0, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP + Y_STEP, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP + Y_STEP, WALL_HEIGHT);
                glVertex3d(x * X_STEP, y * Y_STEP, WALL_HEIGHT);
            }
            if(cell.isFrontier(Cell::N))
            {
                glNormal3d(0.0, 1.0, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP, WALL_HEIGHT);
                glVertex3d(x * X_STEP, y * Y_STEP, WALL_HEIGHT);
            }
            if(cell.isFrontier(Cell::S))
            {
                glNormal3d(0.0, -1.0, 0.0);
                glVertex3d(x * X_STEP, y * Y_STEP + Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP + Y_STEP, 0.0);
                glVertex3d(x * X_STEP + X_STEP, y * Y_STEP + Y_STEP, WALL_HEIGHT);
                glVertex3d(x * X_STEP, y * Y_STEP + Y_STEP, WALL_HEIGHT);
            }

        }
    }

    // Dessin du sol
    GLfloat darkgray[] = {0.2, 0.2, 0.2, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, darkgray);
    glNormal3d(0.0, 0.0, 1.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(mMaze->getWidth() * X_STEP, 0.0, 0.0);
    glVertex3d(mMaze->getWidth() * X_STEP, mMaze->getHeight() * Y_STEP, 0.0);
    glVertex3d(0.0, mMaze->getHeight() * Y_STEP, 0.0);

    // Dessin du plafond, en gardant le materiau du sol
    glNormal3d(0.0, 0.0, -1.0);
    glVertex3d(0.0, 0.0, WALL_HEIGHT);
    glVertex3d(mMaze->getWidth() * X_STEP, 0.0, WALL_HEIGHT);
    glVertex3d(mMaze->getWidth() * X_STEP, mMaze->getHeight() * Y_STEP, WALL_HEIGHT);
    glVertex3d(0.0, mMaze->getHeight() * Y_STEP, WALL_HEIGHT);

    glEnd();

    glPopMatrix();
}
