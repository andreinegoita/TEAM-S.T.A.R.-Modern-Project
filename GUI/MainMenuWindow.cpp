#include "MainMenuWindow.h"
#include "GameWindow.h"
#include <QFile>

MainMenuWindow::MainMenuWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
}

void MainMenuWindow::setPlayerName(const QString& playerName)
{
    welcomeLabel->setText("Hello " + playerName);
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

    welcomeLabel = new QLabel("Hello PlayerName", this);
    welcomeLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    welcomeLabel->setStyleSheet("font-size: 18px; color: black; padding: 10px;");
    welcomeLabel->setGeometry(width() - 310, 10, 300, 30);

    menuButton = new QPushButton("Meniu", this);
    startGameButton = new QPushButton("Start Game", this);

    menuButton->setStyleSheet("font-size: 16px; padding: 10px; border-radius: 4px; background-color: white; color: black;");
    startGameButton->setStyleSheet("font-size: 16px; padding: 10px; border-radius: 4px; background-color: white; color: black;");

    menuButton->setGeometry(500, 400, 200, 50); 
    startGameButton->setGeometry(500, 500, 200, 50); 

    connect(startGameButton, &QPushButton::clicked, this, &MainMenuWindow::startGame);
}

void MainMenuWindow::startGame()
{
    GameWindow* gameWindow = new GameWindow();
    gameWindow->show();
    this->close();
}
