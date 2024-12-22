#pragma once

#ifdef POWERUPSDLL_EXPORTS
#define POWERUPSDLL_API __declspec(dllexport)
#else
#define POWERUPSDLL_API __declspec(dllimport)
#endif

enum class POWERUPSDLL_API PowerUpType {
    SpeedBoost,
    Shield,
    ExtraLife,
    DoubleDamage
};

class POWERUPSDLL_API PowerUp {
public:
    PowerUp(PowerUpType type, int duration);
    PowerUpType GetType() const;
    int GetDuration() const;
    void ApplyEffect();
    void RemoveEffect();

private:
    PowerUpType m_type;
    int m_duration;
};

extern "C" POWERUPSDLL_API PowerUp* createPowerUp(PowerUpType type, int duration);
extern "C" POWERUPSDLL_API void destroyPowerUp(PowerUp* powerUp);