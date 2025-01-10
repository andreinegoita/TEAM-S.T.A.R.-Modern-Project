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
    FireRate,
    BuletUpgrade
};
