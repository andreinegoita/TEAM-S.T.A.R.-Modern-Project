#pragma once
#include<iostream>
#include<cstdint>
#include<vector>
#include<ctime>
#include<random>
#include"CellType.h"

class GameMap
{
public:
	
	GameMap(uint16_t rows, uint16_t cols);
	
	CellType getCellType(uint16_t row, uint16_t col) const;

	uint16_t getRows() const;
	
	uint16_t getCols() const;
	
	void setCellType(uint16_t row, uint16_t col, CellType type);

	void generateMap();

	bool IsInBounds(int x, int y) const;

	void DrawCell(int x, int y, CellType type);

	void UpdatePlayerPosition(uint16_t row, uint16_t col, int newX, int newY);

	friend std::ostream& operator<<(std::ostream& os, const GameMap& gameMap);

private:

	const uint16_t m_rows;
	const uint16_t m_cols; 
	std::vector<std::vector<CellType>> m_map;
	uint16_t m_playerX;
	uint16_t m_playerY;
};

