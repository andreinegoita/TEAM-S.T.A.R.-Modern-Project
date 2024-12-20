#include "PowerUp.h"
#include<iostream>
PowerUp::PowerUp(PowerUpType type, int duration)
    : m_type(type), m_duration(duration) {}

PowerUpType PowerUp::GetType() const {
    return m_type;
}

int PowerUp::GetDuration() const {
    return m_duration;
}

void PowerUp::ApplyEffect() {
    std::cout << "Applying effect of power-up: " << static_cast<int>(m_type) << "\n";
}

void PowerUp::RemoveEffect() {
    std::cout << "Removing effect of power-up: " << static_cast<int>(m_type) << "\n";
}