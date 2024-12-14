#include "GUI.h"
#include <QtWidgets/QApplication>
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <QApplication>
#include "GameWindow.h"
#include "LoginWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    LoginWindow loginWindow;
    loginWindow.setWindowTitle("Battle City Login");
    loginWindow.show();

    GameWindow window;
    window.setWindowTitle("Battle City Map Viewer");
    window.resize(500, 500);
    window.show();

    return app.exec();
}