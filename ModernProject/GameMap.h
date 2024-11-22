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

	GameMap(const GameMap& other);
	
	CellType getCellType(uint16_t row, uint16_t col) const;

	uint16_t getRows() const;
	
	uint16_t getCols() const;
	
	void setCellType(uint16_t row, uint16_t col, CellType type);

	bool isInBounds(int x, int y) const;

	void drawCell(int x, int y, CellType type);

	void updatePlayerPosition(uint16_t row, uint16_t col, int newX, int newY);

	void generateMap();

	friend std::ostream& operator<<(std::ostream& os, const GameMap& gameMap);

private:

	const uint16_t m_rows;
	const uint16_t m_cols; 
	std::vector<std::vector<CellType>> m_map;
	uint16_t m_playerX;
	uint16_t m_playerY;
};

