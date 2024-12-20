#include "PowerUpDll.h"

PowerUp::PowerUp(PowerUpType type, int duration) : m_type{ type }, m_duration{ duration } {}

PowerUpType PowerUp::getType() const {
    return m_type;
}

int PowerUp::getDuration() const {
    return m_duration;
}

void PowerUp::applyEffect() {
    // Implementarea efectului
}

PowerUp* createPowerUp(PowerUpType type, int duration) {
    return new PowerUp(type, duration);
}

void destroyPowerUp(PowerUp* powerUp) {
    delete powerUp;
}
