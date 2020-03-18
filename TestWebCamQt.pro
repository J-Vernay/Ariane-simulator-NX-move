#-------------------------------------------------
#
# Project created by QtCreator 2016-04-13T15:06:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestWebCamQt
TEMPLATE = app
QT       += core gui opengl widgets

# ajout des libs au linker
win32 {
    win32-msvc* {
        LIBS     += opengl32.lib glu32.lib
    } else {
        LIBS     += -lopengl32 -lglu32
    }
}
else {
        LIBS     += -lGL -lGLU
}

SOURCES += main.cpp \
    src/gamelogic.cpp \
    src/models/maze.cpp \
    src/widgets/gamewidget.cpp \
    src/widgets/glscenewidget.cpp \
    src/widgets/mazemapwidget.cpp

HEADERS  += \
    src/gamelogic.hpp \
    src/models/cell.h \
    src/models/maze.h \
    src/widgets/gamewidget.hpp \
    src/widgets/glscenewidget.hpp \
    src/widgets/mazemapwidget.hpp

FORMS    +=

INCLUDEPATH +=$$(OPENCV_DIR)\..\..\include

LIBS += -L$$(OPENCV_DIR)\lib \
    -lopencv_core420 \
    -lopencv_highgui420 \
    -lopencv_imgproc420 \
    -lopencv_imgcodecs420 \
    -lopencv_videoio420 \
    -lopencv_features2d420 \
    -lopencv_calib3d420
