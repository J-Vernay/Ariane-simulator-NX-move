#include "glscenewidget.hpp"
#include <QApplication>
#include <GL/glu.h>
#include <QDesktopWidget>

GLSceneWidget::GLSceneWidget(QWidget *parent) : QGLWidget(parent)
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
    // Reset des tampons
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
