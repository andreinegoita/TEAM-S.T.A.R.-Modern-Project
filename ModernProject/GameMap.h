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
	
	GameMap(size_t rows, size_t cols);

	GameMap(const GameMap& other);
	
	CellType getCellType(size_t row, size_t col) const;

	uint16_t getRows() const;
	
	uint16_t getCols() const;
	
	void setCellType(size_t row, size_t col, CellType type);

	bool IsInBounds(int x, int y) const;

	void DrawCell(int x, int y, CellType type);

	void UpdatePlayerPosition(size_t row, size_t col, int newX, int newY);

	void generateMap();

	friend std::ostream& operator<<(std::ostream& os, const GameMap& gameMap);

private:

	const size_t m_rows;
	const size_t m_cols;
	std::vector<std::vector<CellType>> m_map;
	size_t m_playerX;
	size_t m_playerY;
};

