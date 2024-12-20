#pragma once

enum class PowerUpType {
    SpeedBoost,
    Shield,
    ExtraLife,
    DoubleDamage
};

class PowerUp {
public:
    PowerUp(PowerUpType type, int duration);
    PowerUpType getType() const;
    int getDuration() const;
    void applyEffect();

private:
    PowerUpType m_type;
    int m_duration;
};

extern "C" PowerUp* createPowerUp(PowerUpType type, int duration);
extern "C" void destroyPowerUp(PowerUp* powerUp);
