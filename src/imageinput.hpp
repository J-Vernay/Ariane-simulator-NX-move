#ifndef IMAGEINPUT_HPP
#define IMAGEINPUT_HPP

#include "opencv2/opencv.hpp"
#include <QImage>
#include <thread>

QImage convertToQImage(cv::Mat mat) noexcept;

struct State {
    cv::Mat grayNose;
    cv::Rect faceRect;
    cv::Point noseCenter;

    State() = default;
    // the copy operations do deep-copy
    State(State const& s) { *this = s; }
    State& operator=(State const& s);
};

// processFrame fait le pur traitement d'image : localisation du nez et du visage
State processFrame(cv::Mat frame, State const& previous = {}) noexcept;
// permet de dessiner les infos de debug, la fonction travaille sur une copie qu'elle retourne
cv::Mat drawDebug(cv::Mat frameIn, State const& state) noexcept;

enum class Direction { NONE, UP, DOWN, LEFT, RIGHT, UNKNOWN };
inline char const* getName(Direction d) noexcept {
    static char const* const names[] = { "NONE", "UP", "DOWN", "LEFT", "RIGHT", "UNKNOWN" };
    return names[static_cast<int>(d)];
}
// permet de trouver la direction grâce à l'état récupéré par processFrame
Direction findDirection(State const& state) noexcept;

template<typename T>
cv::Point_<T> getCenter(cv::Rect_<T> const& r) noexcept {
    return (r.tl() + r.br()) / 2;
}

class CameraThread {
public:
    CameraThread(int videoCaptureIndex = 0);
    ~CameraThread() { stop(); }

    bool isActive() { std::unique_lock lg(mMtx); return mActive; }
    void stop() { std::unique_lock lg(mMtx); mActive = false; }
    State getState() { std::unique_lock lg(mMtx); return mState; }
    cv::Mat getFrame() { std::unique_lock lg(mMtx); return mFrame.clone(); }
    Direction getDirection() { std::unique_lock lg(mMtx); return findDirection(mState); }
    std::pair<State, cv::Mat> getStateAndFrame() { std::unique_lock lg(mMtx); return { mState, mFrame.clone() }; }

    int getCameraFrameWidth() const { return mCameraFrameWidth; };
    int getCameraFrameHeight() const { return mCameraFrameHeight; };
private:
    std::mutex mMtx;
    std::thread mThread;
    cv::VideoCapture mVideoCapture;
    cv::Mat mFrame;
    State mState;
    bool mActive;
    const int mCameraFrameWidth = 240, mCameraFrameHeight = 180;
};


#endif // IMAGEINPUT_HPP
