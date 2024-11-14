#include "Player.h"
#include<iostream>

Player::Player(std::string_view  name, std::pair<uint16_t, uint16_t> position, double velocity, DirectionType direction) :m_name{ name },
GameObject{ position, velocity,direction }, m_health{ 3 }, m_points(0), m_score{ 0 } {}
void Player::Shoot()
{
	auto newBullet = m_weapon.fire(m_position.first, m_position.second,m_direction);
	if (newBullet)
	{
		m_bullets.push_back(std::move(newBullet));
        std::cout << this->getNoOfBullets() << "\n";

	}
}

void Player::ResetPosition()
{
	m_position = m_startPosition;
}

void Player::Display()
{
	std::cout << "Player " << m_name << " located at {" << m_position.first << ',' << m_position.second << "} with velocity "
		<< m_speed << " health " << m_health << " points " << m_points << " score " << m_score << std::endl;
}

std::pair<uint16_t, uint16_t> Player::GetStartPosition()
{
	return m_startPosition;
}

void Player::DisplayPosition()
{
	std::cout << "StartPoint:{" << m_startPosition.first << ',' << m_startPosition.second << "}\n";
}

void Player::Draw()
{
	char character ='P';
	printColored(character, Color::YELLOW);
}

void Player::handleInput(const GameMap& gameMap)
{
        if (_kbhit()) {
            char key = _getch(); 
            switch (key) {
            case 'W': case 'w':
                m_direction = DirectionType::Up;
                break;
            case 'A': case 'a':
                m_direction = DirectionType::Left;
                break;
            case 'S': case 's':
                m_direction = DirectionType::Down;
                break;
            case 'D': case 'd':
                m_direction = DirectionType::Right;
                break;
            case ' ': 
                Shoot();
            default:
                return; 
            }

            move(gameMap);
            std::cout << "Position: (" << m_position.first << ", " << m_position.second << ")\n";
        }
    
}

int Player::getNoOfBullets()
{
    return m_bullets.size();
}


