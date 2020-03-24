#include "imageinput.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QDebug>
#include <QDir>
#include <stdexcept>

namespace {

cv::CascadeClassifier loadFaceClassifier() {
    cv::CascadeClassifier classifier;
    qDebug() << QDir::currentPath() << '\n';
    if (!classifier.load("lbpcascade_frontalface_improved.xml"))
        throw std::runtime_error("Erreur en chargeant 'lbpcascade_frontalface_improved.xml'");
    return classifier;
}

cv::CascadeClassifier loadNoseClassifier() {
    cv::CascadeClassifier classifier;
    qDebug() << QDir::currentPath() << '\n';
    if (!classifier.load("haarcascade_mcs_nose.xml"))
        throw std::runtime_error("Erreur en chargeant 'haarcascade_mcs_nose.xml'");
    return classifier;
}

cv::CascadeClassifier& getFaceClassifier() {
    static auto classifier = loadFaceClassifier();
    return classifier;
}

cv::CascadeClassifier& getNoseClassifier() {
    static auto classifier = loadNoseClassifier();
    return classifier;
}

int distSquared(cv::Point a, cv::Point b) {
    cv::Point d = a - b;
    return d.x*d.x + d.y*d.y;
}

cv::Rect findNearest(cv::CascadeClassifier& classifier, cv::Mat img, cv::Rect prev) {
    static std::vector<cv::Rect> potential;
    classifier.detectMultiScale(img, potential);

    switch (potential.size()) {
    case 0: // on reprend la face précédente
        return prev;

    case 1:
        return potential[0];

    default: {// on prend la tête qui est la plus proche de l'ancienne
        cv::Rect faceRect = potential[0];
        cv::Point lastFaceCenter = getCenter(prev);

        int dist = distSquared(lastFaceCenter, getCenter(potential[0]));
        for (cv::Rect const& rect : potential) {
            if (int d = distSquared(lastFaceCenter, getCenter(rect)); d < dist) {
                dist = d;
                faceRect = rect;
            }
        }
        return faceRect;
    }
    }
}

void drawRect(cv::Mat out, cv::Rect rect, cv::Scalar color, int thicknessOut = 2, int thicknessIn = 1) {
    cv::Point
            top = { rect.x + rect.width/2, rect.y },
            bottom = { top.x, top.y + rect.height },
            left = { rect.x, rect.y + rect.height/2 },
            right = { left.x + rect.width, left.y };

    cv::line(out, top, bottom, color, thicknessIn);
    cv::line(out, left, right, color, thicknessIn);
    cv::rectangle(out, rect, color, thicknessOut);
}


}; // namespace

QImage convertToQImage(cv::Mat mat) noexcept {
    // Miroir horizontal pour remettre à gauche de l'image
    // pour que cela corresponde à la gauche de l'utilisateur
    cv::flip(mat ,mat, 1);
    // Passer de BGR à RGB
    cv::cvtColor(mat ,mat, cv::COLOR_BGR2RGB);
    return QImage((const unsigned char*)(mat.data),
                  mat.cols, mat.rows, QImage::Format_RGB888);
}

ImageInput::ImageInput(int index) :
    mCamera{index}
{
    if (!mCamera.isOpened())
        throw std::runtime_error(QString("La caméra n°%1 n'a pas pu être accédée.")
                                        .arg(index).toStdString());
    mCameraSize.width = mCamera.get(cv::CAP_PROP_FRAME_WIDTH);
    mCameraSize.height = mCamera.get(cv::CAP_PROP_FRAME_HEIGHT);
    qDebug() << QString("La caméra n°%1 est ouverte, de dimension %2*%3px.")
                       .arg(index).arg(mCameraSize.width).arg(mCameraSize.height);

}



void ImageInput::makeNewAcquisition() {
    using R = Result;
    R prev = get(&R::faceRect, &R::nose, &R::noseRect);
    R result = prev;

    if (!mCamera.read(result.capture))
        throw std::runtime_error("L'acquisition par la caméra n'a pas pu être réalisée.");

    result.annotatedCapture = result.capture;

    // si prevResult.empty(), alors c'est le rectangle le plus proche du centre qui est choisi
    if (prev.faceRect.empty())
        prev.faceRect = cv::Rect(result.capture.size().width/2, result.capture.size().height/2, 0, 0);

    result.faceRect = findNearest(getFaceClassifier(), result.capture, prev.faceRect);

    if (!result.faceRect.empty()) {
        // on a localisé le visage

        cv::Mat imgFace = result.capture(result.faceRect);
        cv::Rect center{ imgFace.cols / 2, imgFace.rows / 2, 0, 0 };

        if (result.noseRect.empty()) {
            // on a pas de nez disponible : CascadeClassifier pour en trouver un
            result.noseRect = findNearest(getNoseClassifier(), imgFace, center);
            if (!result.noseRect.empty()) {
                // on a localisé le nez
                result.nose = imgFace(result.noseRect).clone();
            }
        } else {
             // on a déjà un nez de disponible : matchTemplate pour le retrouver
            cv::Mat matchResult;
            int result_cols =  imgFace.cols - result.noseRect.width  + 1;
            int result_rows = imgFace.rows - result.noseRect.height + 1;
            matchResult.create(result_cols, result_rows, CV_32FC1);
            cv::Mat faceGray, noseGray;
            cv::cvtColor(imgFace, faceGray, cv::COLOR_BGR2GRAY);
            cv::cvtColor(result.nose, noseGray, cv::COLOR_BGR2GRAY);
            cv::matchTemplate(faceGray, noseGray, matchResult, cv::TM_CCORR_NORMED);
            cv::Point maxLoc;
            cv::minMaxLoc(matchResult, nullptr, nullptr, nullptr, &maxLoc);
            int x = maxLoc.x + result.noseRect.width/2;
            int y = maxLoc.y + result.noseRect.height/2;
            int w = noseGray.size().width, h = noseGray.size().height;
            cv::Point topLeft = { std::max(x - w/2, 0), std::max(y - h/2, 0) };
            cv::Point bottomRight = {
                std::min(topLeft.x + w, imgFace.size().width),
                std::min(topLeft.y + h, imgFace.size().height)
            };
            result.noseRect = cv::Rect(topLeft, bottomRight);
            result.nose = imgFace(result.noseRect).clone();
            cv::imshow("Nose", result.nose);
        }
    }

    cv::Rect noseRectAbsolute {
        result.noseRect.x + result.faceRect.x,
        result.noseRect.y + result.faceRect.y,
        result.noseRect.width, result.noseRect.height };
    drawRect(result.annotatedCapture, result.faceRect, cv::Scalar(0, 0, 255));
    drawRect(result.annotatedCapture, noseRectAbsolute, cv::Scalar(255, 255, 0));

    mLastResultMtx.lock();
    mLastResult = result;
    mLastResultMtx.unlock();
}
