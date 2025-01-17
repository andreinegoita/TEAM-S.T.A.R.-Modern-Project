#include"GUI.h"
#include <QtWidgets/QApplication>
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include <QApplication>
#include "LoginWindow.h"
#include "MainMenuWindow.h"
#include"ShopWindow.h"
#include <QApplication>
#include <cpr/cpr.h>
#include <QTimer>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include"LobbyWindow.h"
#include"GameControler.h"

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


//bool startGameTriggered = false;

//void checkStartCondition(LobbyWindow& lobbyWindow, QApplication app) {
//    cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/get_lobby_players" });
//    if (response.status_code == 200) {
//        QJsonDocument doc = QJsonDocument::fromJson(response.text.c_str());
//        QJsonArray playersArray = doc["players"].toArray();
//
//        int playerCount = playersArray.size();
//        qDebug() << "Players in lobby: " << playerCount;
//
//        if (playerCount >= 2 && !startGameTriggered) {
//            startGameTriggered = true;
//            QTimer* gameStart = new QTimer(&app);
//            gameStart->start(10000);
//            lobbyWindow.startGame();
//        }
//    }
//}

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

    LobbyWindow lobbyWindow;
    lobbyWindow.setWindowTitle("Battle City Lobby");

    GameController gameController(app);

    QObject::connect(&loginWindow, &LoginWindow::loginSuccessful, [&](const QString& playerName) {
        mainMenuWindow.setPlayerName(playerName);
        mainMenuWindow.show();
        //lobbyWindow.show();
        loginWindow.close();

        
       /* QTimer* gameStartTimer = new QTimer(&app);
        QObject::connect(gameStartTimer, &QTimer::timeout, [&]() {
            gameController.checkStartCondition(lobbyWindow, playerName);
            });
        gameStartTimer->start(1000);*/
        });

    loginWindow.show();
    return app.exec();
}