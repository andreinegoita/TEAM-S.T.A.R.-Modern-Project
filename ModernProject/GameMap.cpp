﻿#include "GameMap.h"
#include <crow.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
GameMap::GameMap(size_t rows, size_t cols)
	:m_rows(rows), m_cols(cols), m_map(rows, std::vector<CellType>(cols, CellType::EMPTY)), m_playerX{ 0 }, m_playerY{ 0 } {}


CellType GameMap::getCellType(size_t row, size_t col) const
{
	if (row >= m_rows && col >= m_cols)
		throw std::out_of_range("Cell coordinates are out of bounds");
	return m_map[row][col];
}


void GameMap::setCellType(size_t row, size_t col, CellType type)
{
	if (row < m_rows && col < m_cols)
		m_map[row][col] = type;
	else
		throw("Cell coordinates are out of bounds");
}

bool GameMap::IsInBounds(int x, int y) const
{
	return x >= 0 && x < m_map[0].size() && y >= 0 && y < m_map.size();
}

void GameMap::DrawCell(int x, int y, CellType type)
{
	if (IsInBounds(x, y))
	{
		m_map[y][x] = type;
	}
}

void GameMap::UpdatePlayerPosition(size_t row, size_t col, int newX, int newY)
{
	if (IsInBounds(newX, newY) && m_map[newY][newX] == CellType::EMPTY)
	{
		m_playerX = newX;
		m_playerY = newY;
		DrawCell(newX, newY, CellType::EMPTY);
	}
}

void GameMap::generateMap()
{
	std::mt19937 mt(time(nullptr));
	std::uniform_int_distribution<int> dist(1, 100);

	for (size_t row = 0; row < m_rows; row++) {
		for (size_t col = 0; col < m_cols; col++) {
			if (row == 0 || row == m_rows - 1 || col == 0 || col == m_cols - 1)
				setCellType(row, col, static_cast<CellType>(0)); 
			else {
				int randomPercent = dist(mt);
				if (randomPercent <= 10)
					setCellType(row, col, static_cast<CellType>(2)); 
				else {
					uint16_t randomVal = mt() % 3;
					setCellType(row, col, static_cast<CellType>(randomVal)); 
				}
			}

			if (((col == 0 || col == m_cols - 1) && row == (m_rows - 1) / 2) ||
				(row == 0 || row == m_rows - 1) && col == (m_cols - 1) / 2) {
				setCellType(row, col, static_cast<CellType>(2));
			}
		}
	}

	if (getCellType(0, (m_cols - 1) / 2) == static_cast<CellType>(2)) {
		setCellType(0, (m_cols - 1) / 2 - 1, static_cast<CellType>(0));
		setCellType(0, (m_cols - 1) / 2 + 1, static_cast<CellType>(0));
	}

	if (getCellType(m_rows - 1, (m_cols - 1) / 2) == static_cast<CellType>(2)) {
		setCellType(m_rows - 1, (m_cols - 1) / 2 - 1, static_cast<CellType>(0));
		setCellType(m_rows - 1, (m_cols - 1) / 2 + 1, static_cast<CellType>(0));
	}

	if (getCellType((m_rows - 1) / 2, 0) == static_cast<CellType>(2)) {
		setCellType((m_rows - 1) / 2 - 1, 0, static_cast<CellType>(0));
		setCellType((m_rows - 1) / 2 + 1, 0, static_cast<CellType>(0));
	}

	if (getCellType((m_rows - 1) / 2, m_cols - 1) == static_cast<CellType>(2)) {
		setCellType((m_rows - 1) / 2 - 1, m_cols - 1, static_cast<CellType>(0));
		setCellType((m_rows - 1) / 2 + 1, m_cols - 1, static_cast<CellType>(0));
	}
}



bool GameMap::isValidMap() const {
	auto cells = m_map | std::views::join;

	
	std::vector<CellType> validCellTypes = {
		CellType::EMPTY,
		CellType::BREAKABLE_WALL,
		CellType::Player,
		CellType::UNBREAKABLE_WALL,
		CellType::Bullet,
		CellType::Bomb
	};

	
	bool allCellsValid = std::ranges::all_of(cells, [&validCellTypes](CellType cell) {
		return std::ranges::find(validCellTypes, cell) != validCellTypes.end();
		});

	
	bool playersInBounds = std::ranges::all_of(cells, [this](CellType cell) {
		return cell != CellType::Player || IsInBounds(m_playerX, m_playerY);
		});

	return allCellsValid && playersInBounds;
}


GameMap::GameMap(const GameMap& other) :m_rows(other.m_rows), m_cols(other.m_cols), m_playerX{ 0 }, m_playerY{ 0 } ,m_map(other.m_map)
{
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
	for (size_t row = 0; row < gameMap.m_rows; row++)
	{
		for (size_t col = 0; col < gameMap.m_cols; ++col)
		{
			switch (gameMap.m_map[row][col])
			{
			case CellType::EMPTY: os << ' ' << "\033[34m" << "-" << "\033[0m" << ' ';
				break;
			case CellType::BREAKABLE_WALL: os << ' ' << "\033[33m" << "#" << "\033[0m" << ' ';
				break;
			case CellType::UNBREAKABLE_WALL: os << ' ' << "\033[31m" << "X" << "\033[0m" << ' ';
				break;
			case CellType::Player: os << ' ' << "\033[35m" << "P" << "\033[0m" << ' ';
				break;
			case CellType::Bullet: os << ' ' << "*" << ' ';
				break;
			case CellType::Bomb: os << ' ' << "\033[32m" << "#" << "\033[0m" << ' ';
				break;
			}
		}
			os << "\n";
	}
	return os;
}

void GameMap::UpdateCell(int row, int col, int value) {
	if (row >= 0 && row < m_rows && col >= 0 && col < m_cols) {
		m_map[row][col] = static_cast<CellType>(value);
	}
}

std::string GameMap::GetMapState() const {
	std::ostringstream oss;
	oss << "[";

	for (int i = 0; i < m_rows; ++i) {
		oss << "[";
		for (int j = 0; j < m_cols; ++j) {
			oss << "\"" << CellTypeToString(m_map[i][j]) << "\""; 
			if (j < m_cols - 1) oss << ",";
		}
		oss << "]";
		if (i < m_rows - 1) oss << ","; 
	}

	oss << "]";
	return oss.str();
}
