#include "pch.h"
#include"PowerUps.h"


 PowerUpType Power::StringToPowerUpType(const std::string& str)
 {
     if (str == "SpeedBoost") {
         return PowerUpType::SpeedBoost;
     }
     else if (str == "Shield") {
         return PowerUpType::Shield;
     }
     else if (str == "ExtraLife") {
         return PowerUpType::ExtraLife;
     }
     else if (str == "FireRate") {
         return PowerUpType::FireRate;
     }
     else
         return PowerUpType::FireRate;
 }

