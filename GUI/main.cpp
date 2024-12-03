#include "GUI.h"
#include <QtWidgets/QApplication>

#include <QApplication>
#include "MapWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MapWindow window;
    window.setWindowTitle("Battle City Map Viewer");
    window.resize(500, 500);
    window.show();

    return app.exec();
}