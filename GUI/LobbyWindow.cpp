#include "LobbyWindow.h"
#include "GameWindow.h"


LobbyWindow::LobbyWindow(QWidget* parent) : QMainWindow(parent), playerCount(0), countdownTime(60) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    lobbyLabel = new QLabel("Waiting for players...", this);
    startButton = new QPushButton("Start Game", this);
    startButton->setEnabled(false);  
    countdownTimer = new QTimer(this);

    layout->addWidget(lobbyLabel);
    layout->addWidget(startButton);

    QTimer* playerUpdateTimer = new QTimer(this);
    connect(playerUpdateTimer, &QTimer::timeout, this, &LobbyWindow::updatePlayerList);
    playerUpdateTimer->start(5000);

    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    connect(startButton, &QPushButton::clicked, this, &LobbyWindow::startGame);
    connect(countdownTimer, &QTimer::timeout, this, &LobbyWindow::onPlayerJoined);
}

void LobbyWindow::addPlayer(const QString& playerName) {
    players.push_back(playerName);
    playerCount++;
    lobbyLabel->setText("Players: " + QString::number(playerCount));

    
    if (playerCount >= 2) {
        countdownTime = 60;
        startButton->setEnabled(true);
        countdownTimer->start(1000);  
    }
}

void LobbyWindow::updatePlayerList() {
    cpr::Response response = cpr::Get(cpr::Url{ base_url+"/get_lobby_players" });
    if (response.status_code == 200) {
        QJsonDocument doc = QJsonDocument::fromJson(response.text.c_str());
        QJsonArray playersArray = doc["players"].toArray();

        
        if (playersArray.size() != playerCount) {
            players.clear();
            for (const auto& player : playersArray) {
                players.push_back(player.toString());
            }
            playerCount = players.size();
            lobbyLabel->setText("Players: " + QString::number(playerCount));
        }
    }
    else {
        qDebug() << "Failed to fetch player data!";
    }
}

void LobbyWindow::onPlayerJoined() {
    updatePlayerList();  

   
    if (playerCount >= 0 && (countdownTime == 0 || playerCount == 4)) {
        startGame();
    }

    countdownTime--;
    lobbyLabel->setText("Game starting in: " + QString::number(countdownTime) + " seconds");
}

void LobbyWindow::startGame() {
    countdownTimer->stop();
    GameWindow* gameWindow = new GameWindow();
    gameWindow->show();
    this->close();
}
