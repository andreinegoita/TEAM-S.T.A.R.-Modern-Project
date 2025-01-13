#pragma once



#include <string>
#include <stdexcept>

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
