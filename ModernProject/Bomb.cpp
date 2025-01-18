import Bomb;
#include <ctime>
#include <random>
#include "GameObject.h"
#include "GameMap.h"

Bomb::Bomb() : GameObject{ {0,0}, 0, DirectionType::Up }
{
}

void Bomb::SetCoordinates(GameMap& gameMap)
{
    uint16_t sizeRows = gameMap.getRows(), sizeCols = gameMap.getCols();
    std::mt19937 mt(static_cast<unsigned long>(time(nullptr))); 

    uint16_t randomRow, randomCol;
    while (true)
    {
        randomRow = mt() % sizeRows;
        randomCol = mt() % sizeCols;

        if (gameMap.getCellType(randomRow, randomCol) == CellType::BREAKABLE_WALL)
        {
            this->m_position.first = randomRow;
            this->m_position.second = randomCol;
            gameMap.setCellType(m_position.first, m_position.second, CellType::Bomb);
            break;
        }
    }
}
