#include "MainMenuWindow.h"
#include "GameWindow.h"
#include <QFile>
#include"LobbyWindow.h"
MainMenuWindow::MainMenuWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
}

void MainMenuWindow::setPlayerName(const QString& playerName)
{
    welcomeLabel->setText(playerName);
}

void MainMenuWindow::setupUI() {
    setFixedSize(1200, 900);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QString backgroundImagePath = "battleCity.jpg";
    QFile file(backgroundImagePath);
    if (!file.exists()) {
        qDebug() << "Imaginea de fundal nu a fost gasita: " << backgroundImagePath;
    }
    else {
        centralWidget->setStyleSheet(QString(
            "QWidget {"
            "    background-image: url('%1');"
            "    background-position: center;"
            "    background-repeat: no-repeat;"
            "    background-size: cover;"
            "}"
        ).arg(backgroundImagePath));
    }

    welcomeLabel = new QLabel("Hello", this);
    welcomeLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    welcomeLabel->setStyleSheet("font-size:18px; color: black; padding: 10px;");
    welcomeLabel->setGeometry(width() - 310, 10, 300, 30);

    menuButton = new QPushButton("Meniu", this);
    startGameButton = new QPushButton("Start Game", this);

    menuButton->setStyleSheet(
        "QPushButton {"
        "    font-size: 24px;"
        "    padding: 12px 20px;"
        "    border-radius: 15px;"
        "    background-color: white;"
        "    color: #1E3A5F;"
        "    border: 2px solid #1E3A5F;"
        "    box-shadow: 0 0 10px #1E3A5F, 0 0 20px #1E3A5F, 0 0 30px #1E3A5F;"
        "    transition: all 0.3s ease;"
        "    text-shadow: 0 0 10px #1E3A5F, 0 0 20px #1E3A5F, 0 0 30px #1E3A5F;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.8);"
        "    color: #1E3A5F;"
        "    border: 2px solid #1E3A5F;"
        "    box-shadow: 0 0 15px #1E3A5F, 0 0 25px #1E3A5F, 0 0 35px #1E3A5F;"
        "    text-shadow: 0 0 15px #1E3A5F, 0 0 25px #1E3A5F, 0 0 35px #1E3A5F;"
        "}"
    );

    startGameButton->setStyleSheet(
        "QPushButton {"
        "    font-size: 24px;"
        "    padding: 12px 20px;"
        "    border-radius: 15px;"
        "    background-color: white;"
        "    color: #1E3A5F;"
        "    border: 2px solid #1E3A5F;"
        "    box-shadow: 0 0 10px #1E3A5F, 0 0 20px #1E3A5F, 0 0 30px #1E3A5F;"
        "    transition: all 0.3s ease;"
        "    text-shadow: 0 0 10px #1E3A5F, 0 0 20px #1E3A5F, 0 0 30px #1E3A5F;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.8);"
        "    color: #1E3A5F;"
        "    border: 2px solid #1E3A5F;"
        "    box-shadow: 0 0 15px #1E3A5F, 0 0 25px #1E3A5F, 0 0 35px #1E3A5F;"
        "    text-shadow: 0 0 15px #1E3A5F, 0 0 25px #1E3A5F, 0 0 35px #1E3A5F;"
        "}"
    );

    menuButton->setGeometry(500, 400, 200, 50);
    startGameButton->setGeometry(500, 500, 200, 50);


    connect(startGameButton, &QPushButton::clicked, this, &MainMenuWindow::startGame);

    connect(menuButton, &QPushButton::clicked, this, [this]() {
        ShopWindow* shopWindow = new ShopWindow();
        shopWindow->setWindowTitle("Battle City Menu");
        shopWindow->show();
        this->close();
        });


}

void MainMenuWindow::startGame() {
    QString playerName = welcomeLabel->text();
    LobbyWindow* lobbyWindow = new LobbyWindow(playerName);
    lobbyWindow->addPlayer(welcomeLabel->text()); 

   
    cpr::Post(
        cpr::Url{ base_url+"/join_lobby" },
        cpr::Body{ "{\"name\": \"" + welcomeLabel->text().toStdString() + "\"}" },
        cpr::Header{ { "Content-Type", "application/json" } }
    );

    lobbyWindow->show();
    this->close();
}