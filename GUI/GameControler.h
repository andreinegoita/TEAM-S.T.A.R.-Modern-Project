#include <cpr/cpr.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "GameWindow.h"
#include"LobbyWindow.h"

class GameController {
public:
    explicit GameController(QApplication& app);

    void checkStartCondition(LobbyWindow& lobbyWindow, const QString& playerName);

    void startGame(LobbyWindow& lobbyWindow, const QString& playerName);

private:
    QApplication& app;
    QTimer* countdownTimer;
    int countdownTime;
    bool startGameTriggered;
    std::string base_url = "http://localhost:18080";
};
