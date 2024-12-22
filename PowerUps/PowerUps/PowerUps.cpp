#include "pch.h"
#include "PowerUps.h"

#include <iostream>
PowerUp::PowerUp(PowerUpType type, int duration) : m_type(type), m_duration(duration) {}

PowerUpType PowerUp::GetType() const
{
    return m_type;
}

int PowerUp::GetDuration() const
{
    return m_duration;
}

void PowerUp::ApplyEffect()
{
    {
        switch (m_type) {
        case PowerUpType::SpeedBoost:
            std::cout << "Speed Boost activated for " << m_duration << " seconds!\n";
            break;
        case PowerUpType::Shield:
            std::cout << "Shield activated for " << m_duration << " seconds!\n";
            break;
        case PowerUpType::ExtraLife:
            std::cout << "Extra life gained!\n";
            break;
        case PowerUpType::DoubleDamage:
            std::cout << "Double Damage activated for " << m_duration << " seconds!\n";
            break;
        default:
            break;
        }
    }
}

void PowerUp::RemoveEffect()
{
    std::cout << "Removing effect of power-up: " << static_cast<int>(m_type) << "\n";
}

PowerUp* createPowerUp(PowerUpType type, int duration)
{
    return new PowerUp(type, duration);
}

void destroyPowerUp(PowerUp* powerUp)
{
    delete powerUp;
}
