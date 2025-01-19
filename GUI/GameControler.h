#include <QApplication>
#include <QTimer>
#include <QString>
#include <string>
#include "LobbyWindow.h"
#include "GameWindow.h"
#include <cpr/cpr.h>

class GameController {
public:
    explicit GameController(QApplication& app);

  
    void checkStartCondition(LobbyWindow& lobbyWindow, const QString& playerName);

    
    void startGame(LobbyWindow& lobbyWindow, const QString& playerName);

private:
    QApplication& app;           
    QTimer* countdownTimer;     
    int countdownTime;           
    bool startGameTriggered;     
    std::string base_url = "http://localhost:18080"; 
};