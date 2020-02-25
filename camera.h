#ifndef WIDGET_H
#define WIDGET_H

#include "opencv2/opencv.hpp"
#include <QWidget>
#include <QTimer>

using namespace cv;

namespace Ui {
class Widget;
}

class Camera : public QWidget
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = 0);
    ~Camera();

private:
    Ui::Widget *ui;
    VideoCapture * webCam_;
    QTimer * timer;

private slots:
    void on_timer_interval();
};

#endif // WIDGET_H
