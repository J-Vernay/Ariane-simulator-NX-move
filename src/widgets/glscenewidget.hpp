#ifndef GLSCENEWIDGET_HPP
#define GLSCENEWIDGET_HPP

#include <QOpenGLWidget>

/**
 * @brief Classe d'affichage de la scene 3D OpenGL
 */
class GLSceneWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    // Constructeur
    explicit GLSceneWidget(QWidget *parent = nullptr);

protected:

    // Fonction d'initialisation d'OpenGL
    void initializeGL();

    // Fonction de redimensionnement du widget OpenGL
    void resizeGL(int width, int height);

    // Fonction d'affichage OpenGL
    void paintGL();


};

#endif // GLSCENEWIDGET_HPP
