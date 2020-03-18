#include "glscenewidget.hpp"
#include <QApplication>
#include <GL/glu.h>
#include <QDesktopWidget>

GLSceneWidget::GLSceneWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    //move(QApplication::desktop()->rect().center() - rect().center());
}

void GLSceneWidget::initializeGL()
{
    // Reglage de la couleur de fond
    glColor3ub(0,0,0);

    // Activation du z-buffer
    glEnable(GL_DEPTH_TEST);
}

void GLSceneWidget::resizeGL(int width, int height)
{
    // Definition du viewport (zone d'affichage)
    float zoom = std::min(width/16., height/9.);
    float w = zoom * 16, h = zoom * 9;
    glViewport((width-w)/2, (height-h)/2, w, h);
}

void GLSceneWidget::paintGL()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // Reset des tampons
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
    glColor3ub(255, 0, 0);
    glVertex2d(-1, -1);
    glVertex2d(-1, 1);
    glVertex2d(1, 1);
    glVertex2d(1, -1);
    glEnd();

    glPopMatrix();
}
