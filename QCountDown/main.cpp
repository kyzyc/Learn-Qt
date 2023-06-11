#include <QApplication>

#include "ccwindowwidget.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    CCWindowWidget windowWidget;
    windowWidget.show();

    return a.exec();
}
