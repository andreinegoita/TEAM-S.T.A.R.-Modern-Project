#include "Weapon.h"

Weapon::Weapon():m_fireRate(4.0),m_bulletSpeed(0.25),m_lastFire(std::chrono::steady_clock::now())
{
}
