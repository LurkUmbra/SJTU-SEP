#include <QApplication>

#include "GameWidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GameWidget widget;
    widget.setWindowTitle("QLink");
    widget.show();

    return app.exec();
}
