#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include<iostream>

class ShopWindow : public QWidget {
    Q_OBJECT

public:
    explicit ShopWindow(QWidget* parent = nullptr);
private:
    void setupUI();
    void sendPowerUpRequest(const std::string& powerUpType);
    void styleButton(QPushButton* button);
    void updatePlayerPoints();

    QLabel* shopLabel;
    QLabel* pointsLabel;
    QPushButton* buyPowerSpeed;
    QPushButton* buyPowerShield;
    QPushButton* ExtraLife;
    QPushButton* FireRate;
    int playerPoints = 100; 
};
