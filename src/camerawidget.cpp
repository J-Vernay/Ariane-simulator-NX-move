#include "camerawidget.hpp"

CameraWidget::CameraWidget(ImageInput& input, QWidget *parent) :
    QWidget(parent),
    mInput(input)
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
    mInput.makeNewAcquisition();

    using R = ImageInput::Result;
    R r = mInput.get(&R::annotatedCapture, &R::faceRect);

    mLabel->setPixmap(QPixmap::fromImage(convertToQImage(r.annotatedCapture)));
    mLabel->resize(mLabel->pixmap()->size());
}
