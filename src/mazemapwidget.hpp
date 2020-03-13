#ifndef MAZEMAPWIDGET_HPP
#define MAZEMAPWIDGET_HPP

#include <QWidget>
#include <QGraphicsOpacityEffect>

/**
 * @brief Classe d'affichage de la minimap
 */
class MazeMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MazeMapWidget(QWidget *parent = nullptr);

    // Fonction de dessin
    void paintEvent(QPaintEvent *event) override;

private:
    QGraphicsOpacityEffect mOpacityEffect;

};

#endif // MAZEMAPWIDGET_HPP
