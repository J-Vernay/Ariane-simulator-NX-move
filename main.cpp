#include "src/camerawidget.hpp"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    try {
        QApplication a(argc, argv);
        ImageInput input(0);
        CameraWidget w(input);
        w.show();

        return a.exec();
    } catch (std::exception const& e) {
        qDebug() << e.what() << '\n';
        return 1;
    }
}
