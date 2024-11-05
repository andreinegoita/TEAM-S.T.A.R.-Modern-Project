#pragma once
#include<iostream>
#include<cstdint>
#include<vector>
#include<ctime>
#include"CellType.h"
class GameMap
{
public:

	GameMap(uint16_t rows, uint16_t cols);

	CellType getCellType(uint16_t row, uint16_t col) const;

	void setCellType(uint16_t row, uint16_t col, CellType type);

	void generateMap();

	void displayMap() const;

private:

	const uint16_t rows;
	const uint16_t cols;
	std::vector<std::vector<CellType>> map;

};

