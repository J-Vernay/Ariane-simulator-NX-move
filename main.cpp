#include "src/widgets/gamewidget.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameWidget w;
    w.show();

    return a.exec();
}