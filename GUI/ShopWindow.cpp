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
        qDebug() << "Imaginea de fundal nu a fost găsită: " << backgroundImagePath;
    }
    else {
        setStyleSheet(QString(
            "QWidget {"
            "    background-image: url('%1');"
            "    background-position: center;"
            "    background-repeat: no-repeat;"
            "    background-size: cover;"
            "}"
        ).arg(backgroundImagePath));

    }

    QVBoxLayout* layout = new QVBoxLayout(this);

    shopLabel = new QLabel("Welcome to the Shop", this);
    shopLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(shopLabel);

    buyPowerSpeed = new QPushButton("Buy Speed", this);
    buyPowerSpeed->setStyleSheet(
        "QPushButton {"
        "    font-size: 24px;"
        "    padding: 12px 20px;"
        "    border-radius: 15px;"
        "    background-color: transparent;"
        "    color: black;"
        "    border: 2px solid #00ffff;"
     
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.8);"
        "    color: #1E3A5F;"
        "    border: 2px solid #1E3A5F;"
     
        "}"
        );


    buyPowerShield = new QPushButton("Buy Shield", this);
    buyPowerShield->setStyleSheet(
        "QPushButton {"
        "    font-size: 24px;"
        "    padding: 12px 20px;"
        "    border-radius: 15px;"
        "    background-color: transparent;"
        "    color: black;"
        "    border: 2px solid #00ffff;"
      
        "}"
        "QPushButton:hover {"
        "    background-color:rgba(255, 255, 255, 0.8);"
        "    color: #1E3A5F;"
        "border: 2px solid #1E3A5F; "
       
        "}"
    );

    ExtraLife = new QPushButton("Buy ExtraLife", this);
    ExtraLife->setStyleSheet(
        "QPushButton {"
        "    font-size: 24px;"
        "    padding: 12px 20px;"
        "    border-radius: 15px;"
        "    color: black;"
        "    border: 2px solid #00ffff;"
       
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.8);"
        "    color: #1E3A5F;"
        "    border: 2px solid #1E3A5F;"
        
        "}"
    );

    FireRate = new QPushButton("Buy FireRate", this);
    FireRate->setStyleSheet(
        "QPushButton {"
        "    font-size: 24px;"
        "    padding: 12px 20px;"
        "    border-radius: 15px;"
        "    color: black;"
        "    border: 2px solid #00ffff;"

        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.8);"
        "    color: #1E3A5F;"
        "    border: 2px solid #1E3A5F;"

        "}"
    );


    
    buyPowerSpeed->setGeometry(375,235, 250, 60);
    buyPowerShield->setGeometry(375, 335, 250, 60);
    ExtraLife->setGeometry(375, 435, 250, 60);
    FireRate->setGeometry(375, 535, 250, 60);
  
    QPushButton* exitButton = new QPushButton("Exit to Main Menu", this);
    exitButton->setGeometry(200,100, 200, 50); 

    exitButton->setStyleSheet(
        "QPushButton {"
        "    font-size: 24px;"
        "    padding: 12px 20px;"
        "    border-radius: 15px;"
        "    color: black;"
        "    border: 2px solid #00ffff;"
        "}"
        "QPushButton:hover {"
        "    background-color: rgba(255, 255, 255, 0.8);"
        "    color: #1E3A5F;"
        "    border: 2px solid #1E3A5F;"
        
        "}"
    );


    exitButton->setGeometry(50,800, 250, 60);

    connect(buyPowerSpeed, &QPushButton::clicked, this, [this]() {
        sendPowerUpRequest("SpeedBoost");
        });

    connect(buyPowerShield, &QPushButton::clicked, this, [this]() {
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
    // Creăm JSON-ul pentru cererea de power-up
    QJsonObject jsonObj;
    jsonObj["powerUpType"] = QString::fromStdString(powerUpType);

    // Convertim obiectul JSON într-un șir de caractere
    QJsonDocument jsonDoc(jsonObj);
    QByteArray jsonData = jsonDoc.toJson();

    // Folosim jsonData.toStdString() pentru a crea cererea POST
    cpr::Response r = cpr::Post(cpr::Url{ "http://localhost:18080/buyPowerUp" },
        cpr::Body{ jsonData.toStdString() }, // cererea ca std::string
        cpr::Header{ {"Content-Type", "application/json"} });

    if (r.status_code == 200) {
        std::cout << "Power-up successfully bought!" << std::endl;
        std::cout << "Updated Player State: " << r.text << std::endl;
    }
    else {
        std::cout << "Failed to buy power-up: " << r.status_code << std::endl;
    }
}

