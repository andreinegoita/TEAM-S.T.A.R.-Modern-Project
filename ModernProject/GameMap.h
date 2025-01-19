#pragma once
#include<iostream>
#include<cstdint>
#include<vector>
#include<ctime>
#include<random>
#include"CellType.h"
#include <unordered_map>
#include<ranges>


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

	bool isValidMap() const;

	friend std::ostream& operator<<(std::ostream& os, const GameMap& gameMap);

	void UpdateCell(int row, int col, int value);

	std::string GetMapState() const;



private:

	const size_t m_rows;
	const size_t m_cols;
	std::vector<std::vector<CellType>> m_map;
	size_t m_playerX;
	size_t m_playerY;

	std::unordered_map<std::string, std::pair<size_t, size_t>> m_playerPositions;

	std::string CellTypeToString(CellType cell) const {
		switch (cell) {
		case CellType::EMPTY: return "Empty";
		case CellType::BREAKABLE_WALL: return "Wall";
		case CellType::UNBREAKABLE_WALL: return "Unbreakable";
		case CellType::Player: return "Player";
		case CellType::Bomb: return "Bomb";
		case CellType::Bullet: return "Bullet";
		default: return "Unknown";
		}
	}
};

