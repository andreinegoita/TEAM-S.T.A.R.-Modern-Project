#pragma once
#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <vector>

class LobbyWindow : public QMainWindow {
    Q_OBJECT  

public:
    explicit LobbyWindow(const QString& playerName,QWidget* parent = nullptr);
    void addPlayer(const QString& playerName);
    void startGame();
    void onCountdownTick();

private slots:
    void updatePlayerList();

private:
    QString playerName;
    QLabel* lobbyLabel;
    QPushButton* startButton;
    QTimer* countdownTimer;
    int playerCount;
    int countdownTime;
    std::vector<QString> players;
    bool gameStarted;
    std::string base_url = "http://localhost:18080";
};