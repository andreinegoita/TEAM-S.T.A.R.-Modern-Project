#include "LobbyWindow.h"
#include "GameWindow.h"
#include <cpr/cpr.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

LobbyWindow::LobbyWindow(const QString& player,QWidget* parent)
    : QMainWindow(parent), playerName(player), playerCount(0), countdownTime(10), gameStarted(false) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    lobbyLabel = new QLabel("Waiting for players...", this);
    startButton = new QPushButton("Start Game", this);
    startButton->setEnabled(false);

    layout->addWidget(lobbyLabel);
    layout->addWidget(startButton);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);


    QTimer* playerUpdateTimer = new QTimer(this);
    connect(playerUpdateTimer, &QTimer::timeout, this, &LobbyWindow::updatePlayerList);
    playerUpdateTimer->start(5000);


    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &LobbyWindow::onCountdownTick);

    connect(startButton, &QPushButton::clicked, this, &LobbyWindow::startGame);
    qDebug() << "Lobby initialized for player:" << playerName;
}

void LobbyWindow::addPlayer(const QString& playerName) {
    if (!playerName.isEmpty()&&std::find(players.begin(),players.end(),playerName)==players.end())
    {
        players.push_back(playerName);
        playerCount++;
        lobbyLabel->setText("Players: " + QString::number(playerCount));
    }
}

void LobbyWindow::updatePlayerList() {
    if (gameStarted) return; 

    cpr::Response response = cpr::Get(cpr::Url{ base_url + "/get_lobby_players" });
    if (response.status_code == 200) {
        QJsonDocument doc = QJsonDocument::fromJson(response.text.c_str());
        QJsonArray playersArray = doc["players"].toArray();


        players.clear();
        for (const auto& player : playersArray) {
            if (!player.toString().isEmpty() && std::find(players.begin(), players.end(), player.toString()) == players.end())
                players.push_back(player.toString());
        }
        playerCount = players.size();
        lobbyLabel->setText("Players: " + QString::number(playerCount));


        if (playerCount >= 1 && playerCount <= 4 && !countdownTimer->isActive()) {
            countdownTime = 10;
            countdownTimer->start(1000);
        }
    }
    else {
        qDebug() << "Failed to fetch player list!";
    }
}

void LobbyWindow::onCountdownTick() {
    if (gameStarted) return; 

    countdownTime--;
    lobbyLabel->setText("Game starting in: " + QString::number(countdownTime) + " seconds");

    if (countdownTime <= 0) {
        countdownTimer->stop();
        startGame();
    }
}

void LobbyWindow::startGame() {
    if (gameStarted) return; 

    gameStarted = true; 

    GameWindow* gameWindow = new GameWindow(playerName);

    gameWindow->show();
    this->close();
}