#include "GameMap.h"

GameMap::GameMap(uint16_t rows, uint16_t cols)
	:m_rows(rows), m_cols(cols), m_map(rows, std::vector<CellType>(cols, CellType::EMPTY)), m_playerX{ 0 }, m_playerY{ 0 } {}


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

void GameMap::UpdatePlayerPosition(uint16_t row, uint16_t col, int newX, int newY)
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

	for (int i = 0;i < m_rows;i++) {
		for (int j = 0;j < m_cols;j++) {
			if (i == 0 || i == m_rows - 1 || j == 0 || j == m_cols - 1)
				setCellType(i, j, static_cast<CellType>(0));
			else {
				int randomPercent = dist(mt);
				if (randomPercent <= 10)
					setCellType(i, j, static_cast<CellType>(2));
				else {
					uint16_t randomVal = mt() % 3;
					setCellType(i, j, static_cast<CellType>(randomVal));
				}
			}
			if (((j == 0 || j == m_cols - 1) && i == (m_rows - 1) / 2) || (i == 0 || i == m_rows - 1) && j == (m_cols - 1) / 2) {
				setCellType(i, j, static_cast<CellType>(2));
			}
		}
	}
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
	for (int y = 0; y < gameMap.m_rows; y++)
	{
		for (int x = 0; x < gameMap.m_cols; ++x)
		{
			switch (gameMap.m_map[y][x])
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
