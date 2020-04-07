#-------------------------------------------------
#
# Project created by QtCreator 2016-04-13T15:06:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestWebCamQt
TEMPLATE = app

CONFIG += c++17

SOURCES += main.cpp \
    src/camerawidget.cpp \
    src/imageinput.cpp

HEADERS  += \
    src/camerawidget.hpp \
    src/imageinput.hpp

FORMS    += \
    camera.ui

INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core420 \
    -lopencv_highgui420 \
    -lopencv_imgproc420 \
    -lopencv_imgcodecs420 \
    -lopencv_videoio420 \
    -lopencv_features2d420 \
    -lopencv_calib3d420\
    -lopencv_objdetect420

RESOURCES += \
    res/resources.qrc
