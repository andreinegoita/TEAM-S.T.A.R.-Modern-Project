#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class ShopWindow : public QWidget {
    Q_OBJECT

public:
    explicit ShopWindow(QWidget* parent = nullptr);
private:
    void setupUI();
    void sendPowerUpRequest(const std::string& powerUpType);

    QLabel* shopLabel;
    QPushButton* buyPowerSpeed;
    QPushButton* buyPowerShield;
    QPushButton* ExtraLife;
};
