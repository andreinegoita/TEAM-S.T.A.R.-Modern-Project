#include "Bullet.h"
#include<chrono>
#include<thread>

Bullet::Bullet(uint16_t x, uint16_t y, DirectionType direction, double speed, bool active):
	GameObject({x,y},speed,direction), m_direction(direction), m_active(active)
{}


DirectionType Bullet::getDirection() const
{
	return m_direction;
}


bool Bullet::isActive() const
{
	return m_active;
}

void Bullet::deactivate()
{
	m_active = false;
}

bool Bullet::collide(const GameObject& other)
{
	auto bulletPosition = this->GetPosition();
	auto objectPosition = other.GetPosition();

	double distance = std::sqrt(std::pow(bulletPosition.first - objectPosition.first, 2) +
		std::pow(bulletPosition.second - objectPosition.second, 2));

	const double collisionThreshold = 4.0;
	if (distance < collisionThreshold) {
		return true;
	}

	return false;
}

void Bullet::moveAndCheck(GameMap& gameMap)
{
	static auto lastUpdateTime = std::chrono::steady_clock::now();
	auto updateInterval = std::chrono::seconds(1);
	auto currentTime = std::chrono::steady_clock::now();

	if (currentTime - lastUpdateTime >= updateInterval) {
		lastUpdateTime = currentTime;

		auto offset = getMovementOffset();
		if (!isOutOfBounds(gameMap, m_position.first + offset.first, m_position.second + offset.second)) {
			updatePosition(offset);
			handleCellInteraction(gameMap);
		}
		else {
			handleOutOfBounds(gameMap);
		}

	}
}


void Bullet::updatePosition(const std::pair<uint16_t, uint16_t>& offset)
{
	
	m_position.first += offset.first;
	m_position.second += offset.second;
	
}

void Bullet::handleOutOfBounds(GameMap& gameMap)
{
		auto [oldX, oldY] = m_position;
		gameMap.setCellType(oldX, oldY, CellType::EMPTY);
		deactivate();
}

void Bullet::destroyBreakableWall(GameMap& gameMap, uint16_t oldX, uint16_t oldY)
{
	gameMap.setCellType(m_position.first, m_position.second, CellType::EMPTY);
	if (gameMap.getCellType(oldX, oldY) == CellType::Bullet) {
		gameMap.setCellType(oldX, oldY, CellType::EMPTY);
	}
	deactivate();
}

void Bullet::stopAtWall(GameMap& gameMap, uint16_t oldX, uint16_t oldY)
{
	if (gameMap.getCellType(oldX, oldY) == CellType::Bullet) {
		gameMap.setCellType(oldX, oldY, CellType::EMPTY);
	}
	deactivate();
}

void Bullet::handleCellInteraction(GameMap& gameMap)
{
	uint16_t oldX = m_position.first - getMovementOffset().first;
	uint16_t oldY = m_position.second - getMovementOffset().second;

	CellType cellType = gameMap.getCellType(m_position.first, m_position.second);

	switch (cellType) {
	case CellType::EMPTY:
		moveBullet(gameMap, oldX, oldY);
		break;
	case CellType::BREAKABLE_WALL:
		destroyBreakableWall(gameMap, oldX, oldY);
		break;
	case CellType::UNBREAKABLE_WALL:
		stopAtWall(gameMap, oldX, oldY);
		break;
	case CellType::Bomb:
		destroyNearbyWalls(gameMap, m_position.first, m_position.second);
		break;
	default:
		break;
	}
}

void Bullet::moveBullet(GameMap& gameMap, uint16_t oldX, uint16_t oldY)
{
	gameMap.setCellType(oldX, oldY, CellType::EMPTY);
	gameMap.setCellType(m_position.first, m_position.second, CellType::Bullet);
}

void Bullet::destroyNearbyWalls(GameMap& gameMap, uint16_t x, uint16_t y)
{
	std::pair<uint16_t, uint16_t> coordinates;
	bool isAnotherBomb = false;
	for (int i = x - 1; i <= x + 1; i++)
		for (int j = y - 1; j <= y + 1; j++)
			if (gameMap.getCellType(i, j) == CellType::Bomb && (x != i || y != j))
			{
				isAnotherBomb = true;
				coordinates.first = i;
				coordinates.second = j;
			}
			else if (gameMap.getCellType(i, j) != CellType::UNBREAKABLE_WALL && gameMap.getCellType(i, j) != CellType::Player)
			{
				gameMap.setCellType(i, j, CellType::EMPTY);
			}
	if (isAnotherBomb)
		destroyNearbyWalls(gameMap, coordinates.first, coordinates.second);
	deactivate();
}
