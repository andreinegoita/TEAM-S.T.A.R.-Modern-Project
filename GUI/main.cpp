#include "GUI.h"
#include <QtWidgets/QApplication>
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <QApplication>
#include "GameWindow.h"
#include "LoginWindow.h"
#include "MainMenuWindow.h"
#include"ShopWindow.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

bool initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // Folosește driverul SQLite
    db.setDatabaseName("../ModernProject/game.db"); // Numele fișierului bazei de date (local)

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
        return -1; // Ieșire din aplicație dacă baza de date nu se poate deschide
    }

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