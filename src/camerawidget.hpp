#ifndef CAMERAWIDGET_HPP
#define CAMERAWIDGET_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include "imageinput.hpp"

class CameraWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CameraWidget(ThreadWrapper& tw, QWidget *parent = nullptr);



private slots:
    void reloadCameraDisplay();

private:
    ThreadWrapper& mTW;

    QVBoxLayout* mLayout;
    QLabel* mLabel;
    QTimer* mTimer;
};

#endif // CAMERAWIDGET_HPP
