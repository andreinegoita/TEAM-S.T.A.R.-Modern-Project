#include "ShopWindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <cpr/cpr.h>
#include <iostream>
#include<QFile>
#include"MainMenuWindow.h"

ShopWindow::ShopWindow(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void ShopWindow::setupUI() {
    setFixedSize(1000, 900);

   

    QString backgroundImagePath = "BattleMenu.jpg";  
    QFile file(backgroundImagePath);
    if (!file.exists()) {
        qDebug() << "Imaginea nu a fost găsită: " << backgroundImagePath;
    }
    else {
        QPalette palette;
        QPixmap backgroundImage(backgroundImagePath);


        palette.setBrush(QPalette::Window, QBrush(backgroundImage));


        setPalette(palette);

        setAutoFillBackground(true);
    }


    QVBoxLayout* layout = new QVBoxLayout(this);

    shopLabel = new QLabel("Welcome to the Shop", this);
    shopLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(shopLabel);

    pointsLabel = new QLabel(this);
    pointsLabel->setGeometry(400, 50, 200, 50);
    pointsLabel->setStyleSheet("font-size: 30px; color: black;background-color:cyan;text-align: center; border: 2px solid #00ffff; border-radius: 10px;");
    pointsLabel->setAlignment(Qt::AlignCenter);
    updatePlayerPoints();

    Speed = new QPushButton("Buy Speed", this);
    styleButton(Speed);


    Shield = new QPushButton("Buy Shield", this);
    styleButton(Shield);

    ExtraLife = new QPushButton("Buy ExtraLife", this);
    styleButton(ExtraLife);

    FireRate = new QPushButton("Buy FireRate", this);
    styleButton(FireRate);


    
    Speed->setGeometry(375,235, 250, 60);
    Shield->setGeometry(375, 335, 250, 60);
    ExtraLife->setGeometry(375, 435, 250, 60);
    FireRate->setGeometry(375, 535, 250, 60);
  
    QPushButton* exitButton = new QPushButton("Exit to Main Menu", this);
    exitButton->setGeometry(200,100, 200, 50); 

    styleButton(exitButton);
      


    exitButton->setGeometry(50,800, 250, 60);

    connect(Speed, &QPushButton::clicked, this, [this]() {
        sendPowerUpRequest("SpeedBoost");
        });

    connect(Shield, &QPushButton::clicked, this, [this]() {
        sendPowerUpRequest("Shield");
        });

    connect(ExtraLife, &QPushButton::clicked, this, [this]() {
        sendPowerUpRequest("ExtraLife");
        });
    connect(FireRate, &QPushButton::clicked, this, [this]() {
        sendPowerUpRequest("FireRate");
        });

    connect(exitButton, &QPushButton::clicked, this, [this]() {
        this->close();  
        MainMenuWindow* mainMenuWindow = new MainMenuWindow();  
        mainMenuWindow->setWindowTitle("Main Menu");
        mainMenuWindow->show(); 
        });
}


void ShopWindow::sendPowerUpRequest(const std::string& powerUpType) {
    QJsonObject jsonObj;
    jsonObj["powerUpType"] = QString::fromStdString(powerUpType);

    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson();

    cpr::Response r = cpr::Post(cpr::Url{ base_url+"/buyPowerUp" },
        cpr::Body{ jsonData.toStdString() }, 
        cpr::Header{ {"Content-Type", "application/json"} });

    if (r.status_code == 200) {
        std::cout << "Power-up successfully bought!" << std::endl;
        std::cout << "Updated Player State: " << r.text << std::endl;
        playerPoints -= 10;
        if (playerPoints < 0) playerPoints = 0;
        updatePlayerPoints();   
    }
    else {
        std::cout << "Failed to buy power-up: " << r.status_code << std::endl;
    }
}

void ShopWindow::styleButton(QPushButton* button) {
    button->setStyleSheet(
        "QPushButton {"
        "    font-size: 24px;"
        "    padding: 12px 20px;"
        "    border-radius: 15px;"
        "    background-color: cyan;"
        "    color: black;"
        "    border: 2px solid #00ffff;"

        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.8);"
        "    color: #1E3A5F;"
        "    border: 2px solid #1E3A5F;"

        "}"
    );
}

void ShopWindow::updatePlayerPoints()
{
    cpr::Response r = cpr::Get(cpr::Url{ base_url+"/getPoints" });

    if (r.status_code == 200) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(r.text).toUtf8());
        QJsonObject jsonObj = jsonDoc.object();

        int playerPoints = jsonObj["playerPoints"].toInt();

        
        pointsLabel->setText("Points: " + QString::number(playerPoints));
    }
    else {
        std::cout << "Error, can't points: " << r.status_code << std::endl;
    }
}


