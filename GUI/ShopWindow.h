#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include<iostream>
#include<string>

class ShopWindow : public QWidget {
    Q_OBJECT

public:
    explicit ShopWindow(QWidget* parent = nullptr);
private:
    void setupUI();
    void sendPowerUpRequest(const std::string& powerUpType);
    void styleButton(QPushButton* button);
    void updatePlayerPoints();
    std::string base_url="http://localhost:18080";
    QLabel* shopLabel;
    QLabel* pointsLabel;
    QPushButton* Speed;
    QPushButton* Shield;
    QPushButton* ExtraLife;
    QPushButton* FireRate;
    int playerPoints = 100; 
};
