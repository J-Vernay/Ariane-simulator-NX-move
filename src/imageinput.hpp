#ifndef IMAGEINPUT_HPP
#define IMAGEINPUT_HPP

#include "opencv2/opencv.hpp"
#include <QImage>
#include <mutex>
#include <atomic>
#include <tuple>

QImage convertToQImage(cv::Mat mat) noexcept;

template<typename T>
cv::Point_<T> getCenter(cv::Rect_<T> const& r) noexcept {
    return (r.tl() + r.br()) / 2;
}

// comme cv::Mat mais réalise des copies de valeur au lieu de copies de référence
// avantage : permet de laisser le compilateur créer les opérateurs de copie par défaut
// pour la classe ImageInput::Result (et ne pas devoir utiliser .clone())
// On peut toujours créer une cv::Mat d'une CopiedMat ce qui sera bien une référence
struct CopiedMat : cv::Mat {
    CopiedMat() noexcept {}
    CopiedMat(cv::Mat m) noexcept : Mat(m.clone()) {}
    CopiedMat& operator=(cv::Mat m) noexcept { m.copyTo(*this); return *this; }
};

// par Julien Vernay
class ImageInput {
public:
    enum Direction { NONE, UP, LEFT, DOWN, RIGHT };

    struct Result {
        CopiedMat capture, annotatedCapture, nose;
        cv::Rect faceRect = { 0, 0, 0, 0 }, noseRect = { 0, 0, 0, 0 };
        Direction direction = NONE;
    };

    // lance runtime_error si la caméra n'est pas ouverte correctement
    ImageInput(int indexCamera);
    ImageInput(ImageInput const&) = delete;
    ImageInput& operator=(ImageInput const&) = delete;

    // lance runtime_error si on n'a pas pu faire l'acquisition
    // ou qu'une erreur a eu lieu pendant le traitement
    void makeNewAcquisition();

    // thread-safe
    Result getAll() const;

    // thread-safe
    // permet d'éviter de TOUT copier, mais les attributs demandés
    // seront synchronisés entre eux (tandis que faire getLastCapture,
    // getDirection... en appels séparés fait qu'ils peuvent être changés
    // entre deux appels aux getters)
    // Exemple d'utilisation :
    // using R = ImageInput::Result;
    // R r = input.get(&R::lastCapture, &R::direction);
    // r.capture   -> OK
    // r.direction -> OK
    // r.nose      -> Pas copié ! Encore valeur par défaut
    template<typename...Attributes>
    Result get(Attributes ImageInput::Result::* ... attributes) const;

private:
    cv::VideoCapture mCamera;
    cv::Size mCameraSize;

    Result mLastResult;
    mutable std::mutex mLastResultMtx;
};

template<typename...Args>
constexpr int evalAll(Args...) { return sizeof...(Args); }

template<typename...Attributes>
ImageInput::Result ImageInput::get(Attributes ImageInput::Result::* ... attributes) const {
    Result r;
    mLastResultMtx.lock();
    evalAll(r.*attributes = mLastResult.*attributes...);
    mLastResultMtx.unlock();
    return r;
}

#endif // IMAGEINPUT_HPP
