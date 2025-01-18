#include "GameControler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

GameController::GameController(QApplication& app) :app(app), countdownTime(15), startGameTriggered(false) {
    countdownTimer = new QTimer(&app);
}

void GameController::checkStartCondition(LobbyWindow& lobbyWindow, const QString& playerName)
{
    cpr::Response response = cpr::Get(cpr::Url{ base_url + "/get_lobby_players" });
    if (response.status_code == 200) {
        QJsonDocument doc = QJsonDocument::fromJson(response.text.c_str());
        QJsonArray playersArray = doc["players"].toArray();

        int playerCount = playersArray.size();
        qDebug() << "Players in lobby: " << playerCount;

        if (playerCount >= 1 && !startGameTriggered) {
            startGameTriggered = true;
            qDebug() << "Countdown started! Game will start in 15 seconds...";

            countdownTimer->start(1000);
            QObject::connect(countdownTimer, &QTimer::timeout, [&]() {
                countdownTime--;
                lobbyWindow.setWindowTitle("Starting in " + QString::number(countdownTime) + " seconds");

                if (countdownTime <= 0) {
                    countdownTimer->stop();
                    startGame(lobbyWindow, playerName);
                }
                });
        }
    }
}

void GameController::startGame(LobbyWindow& lobbyWindow, const QString& playerName)
{

    GameWindow* gameWindow = new GameWindow();


   /* cpr::Response response = cpr::Post(
        cpr::Url{ "http://localhost:18080/request_spawn" },
        cpr::Body{ "{\"name\": \"" + playerName.toStdString() + "\"}" },
        cpr::Header{ {"Content-Type", "application/json"} }
    );


    if (response.status_code == 200) {
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response.text.c_str());
        QJsonObject jsonObject = jsonResponse.object();

        int x = jsonObject["x"].toInt();
        int y = jsonObject["y"].toInt();


        gameWindow->setPlayerStartPosition(x, y);
        qDebug() << "Player " << playerName << " spawned at position: " << x << ", " << y;
    }
    else {
        qDebug() << "Failed to fetch spawn position from server!";
    }*/


    gameWindow->show();
    lobbyWindow.close();
}