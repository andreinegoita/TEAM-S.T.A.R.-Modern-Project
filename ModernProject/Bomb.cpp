#include "Bomb.h"
#include<cmath>

Bomb::Bomb() :GameObject{ {0,0},0,DirectionType::Up }
{
}

void Bomb::SetCoordinates(GameMap& gameMap)
{
	uint16_t sizeRows = gameMap.getRows(), sizeCols = gameMap.getCols();
	std::mt19937 mt(time(nullptr));
	uint16_t randomRow = mt() % sizeRows;
	uint16_t randomCol = mt() % sizeCols;
	while (true)
	{
		uint16_t randomRow = mt() % sizeRows;
		uint16_t randomCol = mt() % sizeCols;
		if (gameMap.getCellType(randomRow, randomCol) == CellType::BREAKABLE_WALL)
		{
			this->m_position.first = randomRow;
			this->m_position.second = randomCol;
			gameMap.setCellType(m_position.first, m_position.second, CellType::Bomb);
			break;
		}
	}
}
