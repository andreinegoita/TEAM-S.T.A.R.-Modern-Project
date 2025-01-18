#include <QApplication>
#include <QTimer>
#include <QString>
#include <string>
#include "LobbyWindow.h"
#include "GameWindow.h"
#include <cpr/cpr.h>

class GameController {
public:
    explicit GameController(QApplication& app); // Constructor

    // Verifică dacă există condițiile pentru a porni jocul
    void checkStartCondition(LobbyWindow& lobbyWindow, const QString& playerName);

    // Pornește jocul
    void startGame(LobbyWindow& lobbyWindow, const QString& playerName);

private:
    QApplication& app;           // Referință la aplicația Qt
    QTimer* countdownTimer;      // Cronometru pentru numărătoarea inversă
    int countdownTime;           // Timpul rămas până la pornirea jocului
    bool startGameTriggered;     // Flag pentru a preveni repornirea cronometrului
    std::string base_url = "http://localhost:18080"; // URL-ul serverului
};