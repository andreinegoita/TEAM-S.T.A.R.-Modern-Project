#include "Bullet.h"
#include<chrono>
#include<thread>

Bullet::Bullet(uint16_t x, uint16_t y, DirectionType direction, double speed, bool active):
	GameObject({x,y},speed,direction), m_direction(direction), m_active(active)
{}


DirectionType Bullet::GetDirection() const
{
	return m_direction;
}


bool Bullet::IsActive() const
{
	return m_active;
}

void Bullet::Deactivate()
{
	m_active = false;
}

bool Bullet::Collide(const GameObject& other)
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

void Bullet::MoveAndCheck(GameMap& gameMap)
{
	static auto lastUpdateTime = std::chrono::steady_clock::now();
	auto updateInterval = std::chrono::seconds(1);
	auto currentTime = std::chrono::steady_clock::now();

	if (currentTime - lastUpdateTime >= updateInterval) {
		lastUpdateTime = currentTime;

		auto offset = getMovementOffset();
		if (!isOutOfBounds(gameMap, m_position.first + offset.first, m_position.second + offset.second)) {
			UpdatePosition(offset);
			HandleCellInteraction(gameMap);
		}
		else {
			HandleOutOfBounds(gameMap);
		}

	}
}


void Bullet::UpdatePosition(const std::pair<uint16_t, uint16_t>& offset)
{
	
	m_position.first += offset.first;
	m_position.second += offset.second;
	
}

void Bullet::HandleOutOfBounds(GameMap& gameMap)
{
		auto [oldX, oldY] = m_position;
		gameMap.setCellType(oldX, oldY, CellType::EMPTY);
		Deactivate();
}

void Bullet::DestroyBreakableWall(GameMap& gameMap, uint16_t oldX, uint16_t oldY)
{
	gameMap.setCellType(m_position.first, m_position.second, CellType::EMPTY);
	if (gameMap.getCellType(oldX, oldY) == CellType::Bullet) {
		gameMap.setCellType(oldX, oldY, CellType::EMPTY);
	}
	Deactivate();
}

void Bullet::StopAtWall(GameMap& gameMap, uint16_t oldX, uint16_t oldY)
{
	if (gameMap.getCellType(oldX, oldY) == CellType::Bullet) {
		gameMap.setCellType(oldX, oldY, CellType::EMPTY);
	}
	Deactivate();
}
void Bullet::DestroyPlayer(GameMap& gameMap, uint16_t oldX, uint16_t oldY)
{
	if (gameMap.getCellType(m_position.first, m_position.second) == CellType::Player) {
		gameMap.setCellType(m_position.first, m_position.second, CellType::EMPTY);
	}

	if (gameMap.getCellType(oldX, oldY) == CellType::Bullet) {
		gameMap.setCellType(oldX, oldY, CellType::EMPTY);
	}

	Deactivate();
}

template <typename T>
void Bullet::CheckCollisionWithCell(GameMap& gameMap, T& object, uint16_t x, uint16_t y, CellType targetCellType) {
	if (gameMap.getCellType(x, y) == targetCellType) {
		gameMap.setCellType(x, y, CellType::EMPTY);
		object.Deactivate();
	}
}


void Bullet::HandleCellInteraction(GameMap& gameMap)
{
	uint16_t oldX = m_position.first - getMovementOffset().first;
	uint16_t oldY = m_position.second - getMovementOffset().second;

	CellType cellType = gameMap.getCellType(m_position.first, m_position.second);

	switch (cellType) {
	case CellType::EMPTY:
		MoveBullet(gameMap, oldX, oldY);
		break;
	case CellType::BREAKABLE_WALL:
		DestroyBreakableWall(gameMap, oldX, oldY);
		break;
	case CellType::UNBREAKABLE_WALL:
		StopAtWall(gameMap, oldX, oldY);
		break;
	case CellType::Bomb:
		DestroyNearbyWalls(gameMap, m_position.first, m_position.second);
		break;
	case CellType::Player:
		DestroyPlayer(gameMap, oldX, oldY);
		break;
	case CellType::Bullet:
		CheckBulletCollision(gameMap, m_position.first, m_position.second);
		break;
	default:
		break;
	}
}

void Bullet::MoveBullet(GameMap& gameMap, uint16_t oldX, uint16_t oldY)
{
	gameMap.setCellType(oldX, oldY, CellType::EMPTY);
	if(m_active)
	{ 
	gameMap.setCellType(m_position.first, m_position.second, CellType::Bullet);
	}

}

void Bullet::DestroyNearbyWalls(GameMap& gameMap, uint16_t x, uint16_t y)
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
			else if (gameMap.getCellType(i, j) == CellType::Player)
			{
				gameMap.setCellType(i, j, CellType::EMPTY);
			}
	if (isAnotherBomb)
		DestroyNearbyWalls(gameMap, coordinates.first, coordinates.second);
	Deactivate();
}
