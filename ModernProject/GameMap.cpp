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
