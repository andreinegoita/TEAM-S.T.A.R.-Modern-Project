#include "Weapon.h"

Weapon::Weapon() :m_fireRate(4.0), m_bulletSpeed(m_speed + 0.25), m_lastFire(std::chrono::steady_clock::now()), GameObject({ 0,0 }, 0,DirectionType::Up)
{
}

Weapon::Weapon(uint16_t x, uint16_t y, double fireRate, double bulletSpeed,DirectionType direction):
    m_fireRate{ fireRate }, m_bulletSpeed{ bulletSpeed }, GameObject{ { x,y }, m_speed, direction }
{
}

std::unique_ptr<Bullet> Weapon::fire(uint16_t x,uint16_t y, DirectionType direction)
{
    auto currentTime = std::chrono::steady_clock::now();
    double timeSinceLastFire = std::chrono::duration<double>(currentTime - m_lastFire).count();

    if (timeSinceLastFire >= m_fireRate) {
        m_lastFire = currentTime;
        std::cout << "Pew Pew\n";
        return std::make_unique<Bullet> (x, y, direction, m_bulletSpeed, true);
    }
    else {
        return nullptr;
    }
}

void Weapon::upgradeBulletSpeed(double increment)
{
    m_bulletSpeed += increment;
}

void Weapon::reduceFireRate(double factor)
{
    m_fireRate = std::max(1.0, m_fireRate - factor);
}

double Weapon::getFireRate() const
{
    return m_fireRate;
}

double Weapon::getBulletSpeed() const
{
    return m_bulletSpeed;
}

void Weapon::Display()
{
    std::cout << "Weapon at coordinates{" << m_position.first << ',' << m_position.second << "} with bullet speed " << m_bulletSpeed << " and fire rate " << m_fireRate << "\n";
}

void Weapon::Draw()
{
    return;
}
