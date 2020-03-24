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
    explicit CameraWidget(ImageInput& input, QWidget *parent = nullptr);



private slots:
    void reloadCameraDisplay();

private:
    ImageInput& mInput;
    QVBoxLayout* mLayout;
    QLabel* mLabel;
    QTimer* mTimer;
};

#endif // CAMERAWIDGET_HPP
