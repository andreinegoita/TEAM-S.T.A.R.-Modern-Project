#include "GameManager.h"
#include <iostream>

GameManager::GameManager(std::vector<std::shared_ptr<Player>> players)
    : m_players(std::move(players)) {}

void GameManager::UpdateGameState() {
    if (IsWinConditionMet()) {
        for (const auto& player : m_players) {
            if (player->IsAlive()) {
                std::cout << "Player " << player->GetPositionState() << " is the winner!" << std::endl;
                break;
            }
        }
    }
}

bool GameManager::IsWinConditionMet() const {
    return CountAlivePlayers() == 1;
}

size_t GameManager::CountAlivePlayers() const {
    return std::count_if(m_players.begin(), m_players.end(),
        [](const auto& player) { return player->IsAlive(); });
}
