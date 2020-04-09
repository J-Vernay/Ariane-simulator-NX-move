#include "imageinput.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <stdexcept>
#include <thread>

namespace { // namespace anonyme : les fonctions définies ici ne sont pas accessibles depuis les autres fichiers

cv::CascadeClassifier loadClassifierFromResource(QString resource)
{
    cv::CascadeClassifier classifier;

    QFile file(resource);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        throw std::runtime_error("Erreur en ouvrant la ressource " + resource.toStdString());
    }

    QTextStream in(&file);
    std::string fileStr = in.readAll().toStdString();
    cv::FileStorage fs(fileStr, cv::FileStorage::READ | cv::FileStorage::MEMORY);

    if (!classifier.read(fs.getFirstTopLevelNode()))
    {
        throw std::runtime_error("Erreur en chargeant le xml de " + resource.toStdString());
    }

    return classifier;
}

cv::CascadeClassifier loadFaceClassifier() {
    return loadClassifierFromResource(":/res/xml/frontalface.xml");
}

cv::CascadeClassifier loadNoseClassifier() {
    return loadClassifierFromResource(":/res/xml/nose.xml");
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

nearestRect findNearest(cv::CascadeClassifier& classifier, cv::Mat img, cv::Point lastFaceCenter) {
    static std::vector<cv::Rect> potential;
    static std::vector<int> rejectLevels;
    static std::vector<double> levelWeights;

    classifier.detectMultiScale(img, potential, rejectLevels, levelWeights, 1.1, 3, 0, cv::Size(), cv::Size(), true);

    switch (potential.size()) {
    case 0: // on retourne rien
        return {};

    case 1:
        return { potential[0], rejectLevels[0], levelWeights[0] };

    default: {// on prend la tête qui est la plus proche de l'ancienne
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

cv::Rect grayCorrelation(cv::Mat image, cv::Mat pattern) {
    cv::Mat matchResult;
    int result_cols =  image.cols - pattern.size().width  + 1;
    int result_rows = image.rows - pattern.size().height + 1;
    matchResult.create(result_cols, result_rows, CV_32FC1);

    // on prend la coordonnée qui maximise la corrélation
    cv::matchTemplate(image, pattern, matchResult, cv::TM_SQDIFF);
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

}; // fin du namespace anonyme


QImage convertToQImage(cv::Mat mat) noexcept {
    if (mat.empty()) return QImage();

    // Miroir horizontal pour remettre à gauche de l'image
    // pour que cela corresponde à la gauche de l'utilisateur
    cv::flip(mat ,mat, 1);
    // Passer de BGR à RGB
    cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);

    return QImage((const unsigned char*)(mat.data),
                  mat.cols, mat.rows, QImage::Format_RGB888);
}

State& State::operator=(State const& s) {
    faceRect = s.faceRect;
    noseCenter = s.noseCenter;
    s.grayNose.copyTo(grayNose);
    return *this;
}

State processFrame(cv::Mat frame, State const& previous) noexcept {
    State current;

    // si on a un nez précédent, on cherche le nez le plus proche spatialement de l'ancien
    // sinon, on prend le nez le plus proche du centre de l'image
    auto nose = findNearest(getNoseClassifier(), frame,
                 (previous.grayNose.empty() ? cv::Point(frame.size().width, frame.size().height) : previous.noseCenter));

    // si on a détecté le nez avec suffisamment de confiance
    if (!nose.object.empty() && nose.levelWeight >= 1.8) {
        auto noseCenter = getCenter(nose.object);
        // avoir détecté le nez avec le classifieur en cascade signifie qu'on est probablement
        // en position neutre, au quel cas, on recherche le visage
        auto face = findNearest(getFaceClassifier(), frame, noseCenter);

        current.noseCenter = getCenter(nose.object);
        cv::cvtColor(frame(nose.object), current.grayNose, cv::COLOR_BGR2GRAY);
        current.faceRect = (!face.object.empty() && face.levelWeight > 1.5) ?
                face.object : // si on a détecté le visage avec suffisamment de confiance
                previous.faceRect; // sinon on reprend l'ancien visage
    }
    // sinon, on a pas détecté de nez avec le classifieur
    // au quel cas, si on a le nez précédent, on le recherche par corrélation dans la nouvelle image
    else if (!previous.grayNose.empty()) {
        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
        auto noseRect = grayCorrelation(grayFrame, previous.grayNose);
        current.grayNose = grayFrame(noseRect).clone();
        current.noseCenter = getCenter(noseRect);
        current.faceRect = previous.faceRect;
    } else {
        current = previous;
    }

    return current;
}

cv::Mat drawDebug(cv::Mat frameIn, State const& state) noexcept {
    if (frameIn.empty()) return cv::Mat();

    cv::Mat frame;
    frameIn.copyTo(frame);

    auto [ frameWidth, frameHeight ] = frame.size();
    auto const& face = state.faceRect;

    // rectangle du visage
    cv::rectangle(frame, state.faceRect, { 0, 0, 255 }, 2);
    // "vecteur" entre le nez et le visage
    cv::line(frame, getCenter(state.faceRect), state.noseCenter, { 255, 255, 0 }, 2);

    cv::line(frame, cv::Point(face.x + 0.3*face.width, 0),
                    cv::Point(face.x + 0.3*face.width, frameHeight), { 0, 255, 0 }, 1);
    cv::line(frame, cv::Point(face.x + 0.7*face.width, 0),
                    cv::Point(face.x + 0.7*face.width, frameHeight), { 0, 255, 0 }, 1);
    cv::line(frame, cv::Point(0, face.y + 0.3*face.height),
                    cv::Point(frameWidth, face.y + 0.3*face.height), { 0, 255, 0 }, 1);
    cv::line(frame, cv::Point(0, face.y + 0.65*face.height),
                    cv::Point(frameWidth, face.y + 0.65*face.height), { 0, 255, 0 }, 1);

    return frame;
}

Direction findDirection(State const& state) noexcept {
    Direction direction = Direction::UNKNOWN;

    if (!state.faceRect.empty()) {
        cv::Point_<float> relativePos = (state.noseCenter - getCenter(state.faceRect));
        relativePos.x /= state.faceRect.width;
        relativePos.y /= state.faceRect.height;

        if (std::hypot(relativePos.x, relativePos.y) > 1) {
            direction = Direction::UNKNOWN; // abérrant que le nez soit très loin du visage
        } else {
            if      (relativePos.y < -0.2 ) direction = Direction::UP;
            else if (relativePos.y >  0.15) direction = Direction::DOWN;
            else if (relativePos.x < -0.2 ) direction = Direction::RIGHT;
            else if (relativePos.x >  0.2 ) direction = Direction::LEFT;
            else                            direction = Direction::NONE;
        }
    }

    return direction;
}

CameraThread::CameraThread(int videoCaptureIndex) :
    mVideoCapture(videoCaptureIndex),
    mActive(mVideoCapture.isOpened())
{
    if (mActive) {
        mVideoCapture.set(cv::CAP_PROP_FRAME_WIDTH,mCameraFrameWidth);
        mVideoCapture.set(cv::CAP_PROP_FRAME_HEIGHT,mCameraFrameHeight);

        mThread = std::thread([&] {
            cv::Mat frame;
            while (true) {
                this->mMtx.lock();
                if (!this->mActive)
                    break;
                State prevState = this->mState;
                this->mMtx.unlock();

                if (!mVideoCapture.read(frame))
                    break;
                State nextState = processFrame(frame, prevState);

                this->mMtx.lock();
                this->mState = nextState;
                frame.copyTo(this->mFrame);
                this->mMtx.unlock();

                // indication pour dire au processeur qu'on peut attendre un peu
                std::this_thread::yield();
            }
            this->mMtx.lock();
            this->mActive = false;
            this->mMtx.unlock();
        });
    }
}
