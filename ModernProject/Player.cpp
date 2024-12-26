#include "Player.h"
#include<iostream>
#include<chrono>
#include<cstdlib>
#include<thread>
#include<sstream>

Player::Player(std::string_view  name, std::pair<uint16_t, uint16_t> position, double velocity, DirectionType direction) :m_name{ name },
GameObject{ position, velocity,direction }, m_health{ 3 }, m_points(1000), m_score{ 0 }, m_startPosition{ position } {}
void Player::Shoot()
{
    auto fireRate = m_weapon.GetFireRate();

    auto currentTime = std::chrono::steady_clock::now();
    auto timeSinceLastShoot = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_lastShootTime);

    if (timeSinceLastShoot.count() >= fireRate) {
        int bulletsToShoot =m_bullets.size()+1;

        for (int i = 0; i < bulletsToShoot; ++i) {

            auto newBullet = m_weapon.Fire(m_position.first, m_position.second, m_direction);
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

void Player::HandleInput( GameMap& gameMap)
{

    gameMap.setCellType(m_position.first, m_position.second, CellType::Player);

        if (_kbhit()) {
            char key = _getch(); 
            switch (key) {
            case 'W': case 'w':
                m_direction = DirectionType::Up;
                Move(gameMap);
                break;
            case 'A': case 'a':
                m_direction = DirectionType::Left;
                Move(gameMap);
                break;
            case 'S': case 's':
                m_direction = DirectionType::Down;
                Move(gameMap);
                break;
            case 'D': case 'd':
                m_direction = DirectionType::Right;
                Move(gameMap);
                break;
            case ' ': 
                Shoot();
                UpdateBullets(gameMap);
                break;
            case 'P': case'p':
                ActivatePowerUp();
                break;
            default:
                return;
            }

        }
    
}

void Player::UpdateBullets(GameMap& gameMap) {

    gameMap.setCellType(m_position.first, m_position.second, CellType::Player);
    for (auto it = m_bullets.begin(); it != m_bullets.end(); ) {
        (*it)->MoveAndCheck(gameMap);


        if (!(*it)->IsActive()) {
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

std::string Player::GetPositionState() const {
    std::ostringstream oss;
    oss << "{";
    oss << "\"y\":" << m_position.first << ",";
    oss << "\"x\":" << m_position.second;
    oss << "}";
    return oss.str();
}

void Player::IncreaseSpeed(double multiplier) {
    m_velocityMultiplier *= multiplier;
    std::cout << "Speed increased by multiplier: " << m_velocityMultiplier << "\n";
}

void Player::ResetSpeed() {
    m_velocityMultiplier = 1.0;
    std::cout << "Speed reset to normal.\n";
}

void Player::ActivateShield() {
    m_shieldActive = true;
    std::cout << "Shield activated! Player is invincible.\n";
}

void Player::DeactivateShield() {
    m_shieldActive = false;
    std::cout << "Shield deactivated! Player is no longer invincible.\n";
}

void Player::GainExtraLife() {
    m_health++;
    std::cout << "Extra life gained! Lives: " <<m_health << "\n";
}

void Player::BuyPowerUp(PowerUpType powerUpType) {
    int cost = 0;

    // Definirea costului pentru fiecare power-up
    switch (powerUpType) {
    case PowerUpType::SpeedBoost:
        cost = 100; // Exemplu de cost
        break;
    case PowerUpType::Shield:
        cost = 150; // Exemplu de cost
        break;
    case PowerUpType::ExtraLife:
        cost = 200; // Exemplu de cost
        break;
    default:
        break;
    }

    if (CanAffordPowerUp(cost)) {
        m_points -= cost;
        m_powerUpQueue.push(powerUpType);
    }
    else {
        std::cout << "You don't have enough money to boy this powerUp!" << std::endl;
    }
}


void Player::ApplyPowerUpEffect(PowerUpType powerUp) {
    switch (powerUp) {
    case PowerUpType::SpeedBoost:
        IncreaseSpeed(1.5); 
        break;
    case PowerUpType::Shield:
        ActivateShield(); 
        break;
    case PowerUpType::ExtraLife:
        GainExtraLife(); 
        break;
    default:
        break;
    }
}

void Player::ActivatePowerUp() {
    if (!m_powerUpQueue.empty()) {
        PowerUpType powerUpType = m_powerUpQueue.front();
        m_powerUpQueue.pop(); // Îndepărtează power-up-ul activat

        // Aplică efectul power-up-ului
        ApplyPowerUpEffect(powerUpType);
    }
}


std::string Player::GetPowerUpState() const {
    std::ostringstream oss;
    oss << "{\"player\": {";
    oss << "\"name\": \"" << m_name << "\", ";
    oss << "\"points\": " << m_points << ", ";
    oss << "\"powerUpQueue\": [";

    std::queue<PowerUpType> tempQueue = m_powerUpQueue; 
    while (!tempQueue.empty()) {
        PowerUpType powerUp = tempQueue.front();
        tempQueue.pop();
        std::string powerUpStr;

        switch (powerUp) {
        case PowerUpType::SpeedBoost:
            powerUpStr = "SpeedBoost";
            break;
        case PowerUpType::Shield:
            powerUpStr = "Shield";
            break;
        case PowerUpType::ExtraLife:
            powerUpStr = "ExtraLife";
            break;
        default:
            break;
        }

        oss << "\"" << powerUpStr << "\"";
        if (!tempQueue.empty()) {
            oss << ",";
        }
    }

    oss << "]}}"; 
    return oss.str();
}
