#include "GameMap.h"

GameMap::GameMap(uint16_t rows, uint16_t cols)
	:rows(rows), cols(cols), map(rows, std::vector<CellType>(cols, CellType::EMPTY))
{
}

CellType GameMap::getCellType(uint16_t row, uint16_t col) const
{
	if (row < rows && col < cols)
		throw std::out_of_range("Cell coordinates are out of bounds");
	return map[row][col];
}

void GameMap::setCellType(uint16_t row, uint16_t col, CellType type)
{
	if (row < rows && col < cols)
		map[row][col] = type;
	else
		throw("Cell coordinates are out of bounds");
}

void GameMap::generateMap()
{
	srand(time(0));
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			uint16_t randomVal = rand() % 3;
			setCellType(i, j, static_cast<CellType>(randomVal));
		}
	}
}

void GameMap::displayMap() const
{
	for(const auto& line:map)
	{
		for(const auto& cell:line)
		{
			switch (cell)
			{
			case CellType::EMPTY: std::cout << " . ";
				break;
			case CellType::BREAKABLE_WALL: std::cout << " # ";
				break;
			case CellType::UNBREAKABLE_WALL: std::cout << " X ";
			}
		}
		std::cout << "\n";
	}
}

std::ostream& operator<<(std::ostream& os, const GameMap& gameMap)
{
	for (const auto& line : gameMap.map)
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
