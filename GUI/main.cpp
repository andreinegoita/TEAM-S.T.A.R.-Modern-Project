#include "GUI.h"
#include <QtWidgets/QApplication>
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <QApplication>
#include "GameWindow.h"
#include "LoginWindow.h"
#include "MainMenuWindow.h"
#include"ShopWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    LoginWindow loginWindow;
    loginWindow.setWindowTitle("Battle City Login");

    MainMenuWindow mainMenuWindow;
    mainMenuWindow.setWindowTitle("Battle City Main Menu");
    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful, [&](const  QString& playerName) {
        mainMenuWindow.setPlayerName(playerName);
        mainMenuWindow.show();
        loginWindow.close();
        });
    ShopWindow shopWindow;
 
    loginWindow.show();

    return app.exec();
}