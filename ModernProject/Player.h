#pragma once
#include<iostream>
#include<string_view>
#include<cstdint>
#include <conio.h>
#include"GameMap.h"
#include"GameObject.h"
#include"Weapon.h"
#include"PowerUps.h"
#include<queue>

class Player:public GameObject
{
public:
	Player( std::string_view name,std::pair<uint16_t,uint16_t>position,double velocity,DirectionType direction);
	void Shoot();

	void ResetPosition();

	std::pair<uint16_t, uint16_t>GetStartPosition();

	void HandleInput(GameMap& gameMap);

	void UpdateBullets(GameMap& gameMap);

	std::string GetPositionState() const;

	void IncreaseSpeed(double multiplier);
	void ResetSpeed();
	void ActivateShield();
	void DeactivateShield();
	void GainExtraLife();

	void ApplyPowerUpEffect(PowerUpType powerUp);
	void BuyPowerUp(PowerUpType powerUpType);
	void ActivatePowerUp();

	bool CanAffordPowerUp(int cost) const { return m_points >= cost; }
	void DeductPoints(int cost) { m_points -= cost; }

	std::string GetPowerUpState() const;



private:
	std::chrono::steady_clock::time_point m_lastShootTime;
	std::string_view m_name;
	std::uint16_t m_health;
	int m_score;
	int m_points;
	Weapon m_weapon;
	std::vector<std::unique_ptr<Bullet>> m_bullets;
	std::pair<uint16_t, uint16_t>m_startPosition;

	double m_velocityMultiplier = 1.0;  
	bool m_shieldActive = false;
	std::queue<PowerUpType> m_powerUpQueue;

};