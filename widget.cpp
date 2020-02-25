#include "opencv2/opencv.hpp"
#include "widget.h"
#include "ui_widget.h"
#include <QString>

using namespace cv;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    timer = new QTimer(this);
    timer->setInterval(1000/60);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer_interval()));

    ui->setupUi(this);

    webCam_=new VideoCapture(0);
    int width=webCam_->get(CAP_PROP_FRAME_WIDTH);
    int height=webCam_->get(CAP_PROP_FRAME_HEIGHT);

    if(!webCam_->isOpened())  // check if we succeeded
    {
        ui->infoLabel_->setText("Error openning the default camera !");
    }
    else
    {
        ui->infoLabel_->setText(QString("Video ok, image size is %1x%2 pixels").arg(width).arg(height));
    }

}


Widget::~Widget()
{
    delete ui;
    delete webCam_;
}


void Widget::on_timer_interval()
{
    if (webCam_->isOpened()) {
        Mat image;
        if (webCam_->read(image)) {   // Capture a frame
            // Flip to get a mirror effect
            flip(image,image,1);
            // Invert Blue and Red color channels
            cvtColor(image,image,COLOR_BGR2RGB);
            // Convert to Qt image
            QImage img= QImage((const unsigned char*)(image.data),image.cols,image.rows,QImage::Format_RGB888);
            // Display on label
            ui->imageLabel_->setPixmap(QPixmap::fromImage(img));
            // Resize the label to fit the image
            ui->imageLabel_->resize(ui->imageLabel_->pixmap()->size());

        }
        else {
            ui->infoLabel_->setText("Error capturing the frame");
        }
    }
}
