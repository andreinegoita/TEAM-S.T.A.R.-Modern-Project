#include "Player.h"
#include<iostream>
#include<chrono>
#include<cstdlib>
#include<thread>

Player::Player(std::string_view  name, std::pair<uint16_t, uint16_t> position, double velocity, DirectionType direction) :m_name{ name },
GameObject{ position, velocity,direction }, m_health{ 3 }, m_points(0), m_score{ 0 } {}
void Player::Shoot()
{
    auto fireRate = m_weapon.getFireRate();

    auto currentTime = std::chrono::steady_clock::now();
    auto timeSinceLastShoot = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_lastShootTime);

    if (timeSinceLastShoot.count() >= fireRate) {
        int bulletsToShoot =m_bullets.size()+1;

        for (int i = 0; i < bulletsToShoot; ++i) {

            auto newBullet = m_weapon.fire(m_position.first, m_position.second, m_direction);
            if (newBullet)
            {
                m_bullets.push_back(std::move(newBullet));
            }
        }
        m_lastShootTime = currentTime;
    }
}

void Player::ResetPosition()
{
	m_position = m_startPosition;
}

std::pair<uint16_t, uint16_t> Player::GetStartPosition()
{
	return m_startPosition;
}

void Player::handleInput( GameMap& gameMap)
{

    gameMap.setCellType(m_position.first, m_position.second, CellType::Player);

        if (_kbhit()) {
            char key = _getch(); 
            switch (key) {
            case 'W': case 'w':
                m_direction = DirectionType::Up;
                move(gameMap);
                break;
            case 'A': case 'a':
                m_direction = DirectionType::Left;
                move(gameMap);
                break;
            case 'S': case 's':
                m_direction = DirectionType::Down;
                move(gameMap);
                break;
            case 'D': case 'd':
                m_direction = DirectionType::Right;
                move(gameMap);
                break;
            case ' ': 
                Shoot();
                updateBullets(gameMap);
                break;
            default:
                return;
            }

        }
    
}

void Player::updateBullets(GameMap& gameMap) {

    gameMap.setCellType(m_position.first, m_position.second, CellType::Player);
    for (auto it = m_bullets.begin(); it != m_bullets.end(); ) {
        (*it)->moveAndCheck(gameMap);


        if (!(*it)->isActive()) {
            it = m_bullets.erase(it);
        }
        else {
            ++it;
        }
    }

    system("cls");
    std::cout << gameMap;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

}