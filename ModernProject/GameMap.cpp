#include "GameMap.h"

GameMap::GameMap(uint16_t rows, uint16_t cols)
	:m_rows(rows), m_cols(cols), m_map(rows, std::vector<CellType>(cols, CellType::EMPTY))
{
}

CellType GameMap::getCellType(uint16_t row, uint16_t col) const
{
	if (row >= m_rows && col >= m_cols)
		throw std::out_of_range("Cell coordinates are out of bounds");
	return m_map[row][col];
}

void GameMap::setCellType(uint16_t row, uint16_t col, CellType type)
{
	if (row < m_rows && col < m_cols)
		m_map[row][col] = type;
	else
		throw("Cell coordinates are out of bounds");
}

void GameMap::generateMap()
{
	srand(time(0));
	for (int i = 0;i < m_rows;i++) {
		for (int j = 0;j < m_cols;j++) {
			uint16_t randomVal = rand() % 3;
			setCellType(i, j, static_cast<CellType>(randomVal));
		}
	}
}

GameMap::GameMap(const GameMap& other):m_rows(other.m_rows),m_cols(other.m_cols), m_map(other.m_map)
{
	//Empty
}

uint16_t GameMap::getRows() const
{
	return m_rows;
}

uint16_t GameMap::getCols() const
{
	return m_cols;
}

std::ostream& operator<<(std::ostream& os, const GameMap& gameMap)
{
	for (const auto& line : gameMap.m_map)
	{
		for (const auto& cell : line)
		{
			switch (cell)
			{
			case CellType::EMPTY: os << " . ";
				break;
			case CellType::BREAKABLE_WALL: os << " # ";
				break;
			case CellType::UNBREAKABLE_WALL: os << " X ";
			}
		}
		os << "\n";
	}
	return os;
}
