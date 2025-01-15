#pragma once

#include <vector>
#include <memory>
#include "Player.h"

class GameManager {
public:
    GameManager(std::vector<std::shared_ptr<Player>> players);

    void UpdateGameState();
    bool IsWinConditionMet() const;

private:
    std::vector<std::shared_ptr<Player>> m_players;

    size_t CountAlivePlayers() const;
};
