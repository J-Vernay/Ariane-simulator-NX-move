#include "src/widgets/gamewidget.hpp"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    try {
        QApplication a(argc, argv);
        GameWidget w;
        w.show();

        return a.exec();
    } catch (std::exception const& e) {
        qDebug() << e.what() << '\n';
        return 1;
    }
}
