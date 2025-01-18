import Weapon;
import <chrono>;

Weapon::Weapon() :m_fireRate(500),m_bulletSpeed(m_speed + 5.0f), m_lastFire(std::chrono::steady_clock::now()), GameObject({ 0,0 }, 0,DirectionType::Up)
{
}

Weapon::Weapon(uint16_t x, uint16_t y, double fireRate, double bulletSpeed,DirectionType direction):
    m_fireRate{ fireRate }, m_bulletSpeed{ bulletSpeed }, GameObject{ { x,y }, m_speed, direction }
{
}

std::unique_ptr<Bullet> Weapon::Fire(uint16_t x,uint16_t y, DirectionType direction)
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

void Weapon::UpgradeBulletSpeed(double increment)
{
    m_bulletSpeed += increment;
}

void Weapon::ReduceFireRate(double factor)
{
    m_fireRate = std::max(1.0, m_fireRate - factor);
}

double Weapon::GetFireRate() const
{
    return m_fireRate;
}

double Weapon::GetBulletSpeed() const
{
    return m_bulletSpeed;
}

void Weapon::SetBulletSpeed(double speed)
{
    m_bulletSpeed = speed;
}

