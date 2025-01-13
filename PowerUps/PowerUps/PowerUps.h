#pragma once
#include <string>

enum class __declspec(dllexport) PowerUpType {
    SpeedBoost,
    Shield,
    ExtraLife,
    FireRate
};

namespace Power
{

    __declspec(dllexport) PowerUpType StringToPowerUpType(const std::string& str);
}