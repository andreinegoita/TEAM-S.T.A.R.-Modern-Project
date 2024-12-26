#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include"ShopWindow.h"

class MainMenuWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainMenuWindow(QWidget* parent = nullptr);
    void setPlayerName(const QString& playerName);

private:
    void setupUI();

    QLabel* backgroundLabel;
    QLabel* welcomeLabel;
    QPushButton* menuButton;
    QPushButton* startGameButton;

    void startGame();
};
