#include "Bullet.h"

Bullet::Bullet(double x, double y, DirectionType direction, double speed, bool active):
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

void Bullet::Display()
{
	std::cout << 0;
}

void Bullet::Draw()
{
	
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

void Bullet::moveAndCheck(GameMap& gameMap, std::vector<GameObject*>& objects)
{
	if (isOutOfBounds(gameMap,m_position.first,m_position.second)) {
		std::cout<<"Bullet is out of bounds"<<std::endl;
		deactivate();
		return;
	}

	move(gameMap);

	for (auto& obj : objects) {
		if (this != obj && collide(*obj)) {
			std::cout << "Bullet collided with another object!" << "\n";
			deactivate();
			break;
		}
	}
}

void Bullet::updatePosition(const std::pair<uint16_t, uint16_t>& offset)
{
	
		m_position.first += offset.first;
		m_position.second += offset.second;
		std::cout << "Bullet moved to: (" << m_position.first << ", " << m_position.second << ")\n";
	
}

void Bullet::handleOutOfBounds(GameMap& gameMap)
{
		auto [oldX, oldY] = m_position;
		gameMap.setCellType(oldX, oldY, CellType::EMPTY);
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
	default:
		break;
	}
}

void Bullet::moveBullet(GameMap& gameMap, uint16_t oldX, uint16_t oldY)
{
	gameMap.setCellType(oldX, oldY, CellType::EMPTY);
	gameMap.setCellType(m_position.first, m_position.second, CellType::Bullet);
}
