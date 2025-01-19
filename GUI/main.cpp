#include"GUI.h"
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include "LoginWindow.h"
#include "MainMenuWindow.h"
#include "LobbyWindow.h"
#include "GameControler.h"
#include <QtWidgets/QApplication>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <cpr/cpr.h>

bool initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../ModernProject/game.db");

    if (!db.open()) {
        qDebug() << "Database Error: " << db.lastError().text();
        return false;
    }
    qDebug() << "Database connected successfully!";
    return true;
}




int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    if (!initializeDatabase()) {
        qDebug() << "Failed to connect to database. Exiting...";
        return -1;
    }

    LoginWindow loginWindow;
    loginWindow.setWindowTitle("Battle City Login");

    MainMenuWindow mainMenuWindow;
    mainMenuWindow.setWindowTitle("Battle City Main Menu");

    GameController gameController(app);

    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful, [&](const QString& playerName) {
        mainMenuWindow.setPlayerName(playerName);
        mainMenuWindow.show();
        loginWindow.close();
        });

    loginWindow.show();
    return app.exec();
}