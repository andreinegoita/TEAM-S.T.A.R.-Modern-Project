#include "ShopWindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <cpr/cpr.h>
#include <iostream>

ShopWindow::ShopWindow(QWidget* parent) : QWidget(parent) {
    setupUI();
}

void ShopWindow::setupUI() {
    setFixedSize(400, 300);

    QVBoxLayout* layout = new QVBoxLayout(this);

    shopLabel = new QLabel("Welcome to the Shop", this);
    shopLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(shopLabel);

    buyPowerSpeed = new QPushButton("Buy Speed", this);
    buyPowerShield = new QPushButton("Buy Shield", this);
    ExtraLife = new QPushButton("Buy ExtraLife", this);

    layout->addWidget(buyPowerSpeed);
    layout->addWidget(buyPowerShield);
    layout->addWidget(ExtraLife);

    connect(buyPowerSpeed, &QPushButton::clicked, this, [this]() {
        sendPowerUpRequest("SpeedBoost");
        });

    connect(buyPowerShield, &QPushButton::clicked, this, [this]() {
        sendPowerUpRequest("Shield");
        });

    connect(ExtraLife, &QPushButton::clicked, this, [this]() {
        sendPowerUpRequest("ExtraLife");
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

