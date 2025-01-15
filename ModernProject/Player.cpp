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
    m_weapon.UpgradeBulletSpeed(multiplier);
    speedBoostStartTime = std::chrono::steady_clock::now();
    std::cout << "Speed increased by multiplier: " << multiplier << "\n";
}

void Player::ReduceFireRate(double factor)
{
    m_weapon.ReduceFireRate(factor);
}

void Player::ActivateShield() {
    m_shieldActive = true;
    shieldStartTime = std::chrono::steady_clock::now();
    std::cout << "Shield activated! Player is invincible.\n";
}

void Player::updatePowerUps()
{
    if (m_shieldActive && std::chrono::steady_clock::now() - shieldStartTime > std::chrono::seconds(10)) {
        m_shieldActive = false;
    }

    if (std::chrono::steady_clock::now() - speedBoostStartTime > std::chrono::seconds(10)) {
        m_weapon.SetBulletSpeed(5.0f);
    }
}
void Player::GainExtraLife() {
    m_health++;
    std::cout << "Extra life gained! Lives: " << m_health << "\n";
}


void Player::BuyPowerUp(PowerUpType powerUpType) {
    int cost = 0;


    switch (powerUpType) {
    case PowerUpType::SpeedBoost:
        cost = 100;
        break;
    case PowerUpType::Shield:
        cost = 150;
        break;
    case PowerUpType::ExtraLife:
        cost = 200;
        break;
    case PowerUpType::FireRate:
        cost = 500;
        if (m_fireRatePurchased)
        {
            std::cout << "You already buy it!" <<std::endl;
            return;
        }
        break;
    default:
        break;
    }
    
    if (CanAffordPowerUp(cost)) {
        m_points -= cost;
        m_powerUpQueue.push(powerUpType);

        if (powerUpType == PowerUpType::FireRate) {
            m_fireRatePurchased = true; 
        }
    }
    else {
        std::cout << "You don't have enough money to buy this powerUp!" << std::endl;
    }
}

bool Player::CanAffordPowerUp(int cost) const
{
        return m_points >= cost;
}


void Player::ApplyPowerUpEffect(PowerUpType powerUp) {
    switch (powerUp) {
    case PowerUpType::SpeedBoost:
        IncreaseSpeed(2.0f);
        break;
    case PowerUpType::Shield:
        ActivateShield();
        break;
    case PowerUpType::ExtraLife:
        GainExtraLife();
        break;
    case PowerUpType::FireRate:
        ReduceFireRate(250);
        break;
    default:
        break;
    }
    m_powerUpQueue.pop();
}



std::string Player::GetPowerUpState() const {
    std::ostringstream oss;
    oss << "[";

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
        case PowerUpType::FireRate:
            powerUpStr = "FireRate";
            break;
        default:
            break;
        }

        oss << "\"" << powerUpStr << "\"";
        if (!tempQueue.empty()) {
            oss << ",";
        }
    }

    oss << "]";
    return oss.str();
}

bool Player::HasShield()
{
    return m_shieldActive;
}

int Player::GetLives()
{
    return m_health;
}

double Player::GetBulletSpeed()
{
    return m_weapon.GetBulletSpeed();
}

double Player::GetFireRate()
{
   return  m_weapon.GetFireRate();
}

int Player::GetPoints()
{
    return m_points;
}

void Player::setName(const std::string& name)
{
    this->m_name = name;
}

bool Player::IsAlive() const
{
    return m_health > 0;
}

void Player::setPoints(int points)
{
    this->m_points = points;
}
