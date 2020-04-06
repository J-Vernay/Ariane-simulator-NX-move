#include "imageinput.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QDebug>
#include <QDir>
#include <stdexcept>
#include <thread>

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

struct nearestRect { cv::Rect object; int rejectLevel; double levelWeight; };

nearestRect findNearest(cv::CascadeClassifier& classifier, cv::Mat img, cv::Rect prev) {
    static std::vector<cv::Rect> potential;
    static std::vector<int> rejectLevels;
    static std::vector<double> levelWeights;

    classifier.detectMultiScale(img, potential, rejectLevels, levelWeights, 1.1, 3, 0, cv::Size(), cv::Size(), true);

    switch (potential.size()) {
    case 0: // on reprend la face précédente
        return { prev, 0, 0 };

    case 1:
        return { potential[0], rejectLevels[0], levelWeights[0] };

    default: {// on prend la tête qui est la plus proche de l'ancienne
        cv::Point lastFaceCenter = getCenter(prev);

        int dist = distSquared(lastFaceCenter, getCenter(potential[0]));
        int i_min = 0;
        for (std::size_t i = 1; i < potential.size(); ++i) {
            cv::Rect const& rect = potential[i];
            if (int d = distSquared(lastFaceCenter, getCenter(rect)); d < dist) {
                dist = d;
                i_min = i;
            }
        }
        return  { potential[i_min], rejectLevels[i_min], levelWeights[i_min] };
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


cv::Rect correlation(cv::Mat image, cv::Mat pattern) {
    cv::Mat matchResult;
    int result_cols =  image.cols - pattern.size().width  + 1;
    int result_rows = image.rows - pattern.size().height + 1;
    matchResult.create(result_cols, result_rows, CV_32FC1);

    // on fait la corrélation sur les images en niveaux de gris
    cv::Mat faceGray, noseGray;
    cv::cvtColor(image, faceGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(pattern, noseGray, cv::COLOR_BGR2GRAY);

    // on prend la coordonnée qui maximise la corrélation
    cv::matchTemplate(faceGray, noseGray, matchResult, cv::TM_SQDIFF);
    cv::Point extremumLoc;
    cv::minMaxLoc(matchResult, nullptr, nullptr, &extremumLoc, nullptr);
    // les coordonnées retournées sont relatives à (result_cols, result_rows)
    // on les passe dans le repère de imgFace
    int x = extremumLoc.x + pattern.size().width/2;
    int y = extremumLoc.y + pattern.size().height/2;
    int w = pattern.size().width, h = pattern.size().height;
    // on en déduit le nouveau rectangle du nez
    cv::Rect rect = { x-w/2, y-h/2, w, h };
    rect &= cv::Rect({0,0}, image.size()); // pour être sûr que ça ne sorte pas de l'image
    return rect;
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
    mCamera.set(cv::CAP_PROP_FRAME_WIDTH,240);
    mCamera.set(cv::CAP_PROP_FRAME_HEIGHT,180);
    mCameraSize.width = mCamera.get(cv::CAP_PROP_FRAME_WIDTH);
    mCameraSize.height = mCamera.get(cv::CAP_PROP_FRAME_HEIGHT);
    qDebug() << QString("La caméra n°%1 est ouverte, de dimension %2*%3px.")
                       .arg(index).arg(mCameraSize.width).arg(mCameraSize.height);

}


void ImageInput::runParallel() {
    if (!this->mRunningParallel) {
        this->mRunningParallel = true;
        mThread = std::thread([this] {
            while (this->mRunningParallel) {
                makeNewAcquisition();
            }
        });
    }
}

ImageInput::Result ImageInput::getAll() const {
    std::lock_guard lock(mLastResultMtx); // unlock() dans le destructeur
    return mLastResult;
}

void ImageInput::makeNewAcquisition() {
    Result prev = getAll();
    Result result = prev; // les paramètres qui ne sont pas explicitement "setté" sont repris de la frame précédente

    if (!mCamera.read(result.capture))
        throw std::runtime_error("L'acquisition par la caméra n'a pas pu être réalisée.");

    result.annotatedCapture = result.capture;

    Direction dir = Direction::NONE;
    auto nose = findNearest(getNoseClassifier(), result.capture, prev.noseRect);
    if (nose.object.empty() || nose.levelWeight <= 1.8) {
        if (prev.noseRect.empty()) { // on a pas encore trouvé de nez
            dir = Direction::UNKNOWN;
        } else { // on a déjà un nez à chercher par corrélation
            result.noseRect = correlation(result.capture, prev.nose);
            result.nose = result.capture(result.noseRect);
        }
    } else { // on a trouvé le nez donc ça veut dire que le visage est bien en face
        result.noseRect = nose.object;
        result.nose = result.capture(result.noseRect);
        // le visage étant bien en face, on le recherche (on prend le + proche du nez)
        auto face = findNearest(getFaceClassifier(), result.capture, result.noseRect);
        if (!face.object.empty() && face.levelWeight > 1.5) { // on a trouvé un visage : on met à jour
            result.faceRect = face.object;
        }
    }
    if (dir != Direction::UNKNOWN) {
        cv::Point_<float> vecDiff = (getCenter(result.noseRect) - getCenter(result.faceRect));
        vecDiff.x /= result.faceRect.size().width;
        vecDiff.y /= result.faceRect.size().height;

        if (vecDiff.x > 1.5f || vecDiff.x < -1.5f || vecDiff.y > 1.5f || vecDiff.y < -1.5f) {
            dir = Direction::UNKNOWN; // abérrant que le nez soit très loin du visage
        } else {
            if (vecDiff.x < -0.1) dir = Direction::RIGHT;
            else if (vecDiff.x > 0.1) dir = Direction::LEFT;
            else if (vecDiff.y < -0.1) dir = Direction::UP;
            else if (vecDiff.y > 0.05) dir = Direction::DOWN;
        }
        qDebug() << vecDiff.x << " ; " << vecDiff.y << "\n";
    }
    cv::flip(result.annotatedCapture, result.annotatedCapture, 1);
    char const* nomsDir[] = { "NONE", "UP", "LEFT", "DOWN", "RIGHT", "UNKNOWN" };
    cv::putText(result.annotatedCapture, nomsDir[(int)dir], {20,20}, cv::FONT_HERSHEY_PLAIN,
            2, cv::Scalar(255, 0, 255), 2);
    cv::flip(result.annotatedCapture, result.annotatedCapture, 1);

    result.direction = dir;
    drawRect(result.annotatedCapture, result.faceRect, cv::Scalar(0, 0, 255));
    drawRect(result.annotatedCapture, result.noseRect, cv::Scalar(255, 255, 0));

    mLastResultMtx.lock();
    mLastResult = result;
    mLastResultMtx.unlock();
}
