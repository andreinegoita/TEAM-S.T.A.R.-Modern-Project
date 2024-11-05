#include "Weapon.h"

Weapon::Weapon():m_fireRate(4.0),m_bulletSpeed(0.25),m_lastFire(std::chrono::steady_clock::now()),m_x(0),m_y(0),m_direction(0)
{
}

Weapon::Weapon(double x, double y, double fireRate, double bulletSpeed, uint8_t direction):
    m_x(x),m_y(y),m_fireRate(fireRate),m_bulletSpeed(bulletSpeed),m_direction(direction)
{
}

std::unique_ptr<Bullet> Weapon::fire(uint16_t x,uint16_t y, uint8_t direction)
{
    auto currentTime = std::chrono::steady_clock::now();
    double timeSinceLastFire = std::chrono::duration<double>(currentTime - m_lastFire).count();

    if (timeSinceLastFire >= m_fireRate) {
        m_lastFire = currentTime;
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
