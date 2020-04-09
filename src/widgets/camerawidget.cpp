#include "camerawidget.hpp"
#include <stdexcept>
#include <QDebug>

CameraWidget::CameraWidget(ThreadWrapper& tw, QWidget *parent) :
    QWidget(parent),
    mTW(tw)
{
    mLayout = new QVBoxLayout;
    mLabel = new QLabel("Chargement...");

    mLayout->addWidget(mLabel);
    setLayout(mLayout);

    mTimer = new QTimer(this);
    mTimer->setInterval(50);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(reloadCameraDisplay()));
    mTimer->start();
}

void CameraWidget::reloadCameraDisplay() {
    auto [ state, frame ] = mTW.getStateAndFrame();

    auto debugFrame = drawDebug(frame, state);

    mLabel->setPixmap(QPixmap::fromImage(convertToQImage(debugFrame)));
    mLabel->resize(mLabel->pixmap()->size());
    this->resize(mLabel->size());
}
