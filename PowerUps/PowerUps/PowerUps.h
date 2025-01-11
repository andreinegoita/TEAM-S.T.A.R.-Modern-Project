#pragma once

#ifdef POWERUPSDLL_EXPORTS
#define POWERUPSDLL_API __declspec(dllexport)
#else
#define POWERUPSDLL_API __declspec(dllimport)
#endif
#include<string>

enum class POWERUPSDLL_API PowerUpType {
    SpeedBoost,
    Shield,
    ExtraLife,
    FireRate
};

//PowerUpType StringToPowerUpType(const std::string& str) {
//    if (str == "SpeedBoost") {
//        return PowerUpType::SpeedBoost;
//    }
//    else if (str == "Shield") {
//        return PowerUpType::Shield;
//    }
//    else if (str == "ExtraLife") {
//        return PowerUpType::ExtraLife;
//    }
//
//    else {
//       
//        return PowerUpType::SpeedBoost;
//    }
//}

