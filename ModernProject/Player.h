#pragma once
import Weapon;
#include<iostream>
#include<string_view>
#include<cstdint>
#include <conio.h>
#include"GameMap.h"
#include"GameObject.h"
#include"../PowerUps/PowerUps/PowerUps.h"
#include<queue>
#include<chrono>

class Player :public GameObject
{
public:
	Player(std::string_view name, std::pair<uint16_t, uint16_t>position, double velocity, DirectionType direction);
	void Shoot();
	void ResetPosition();

	std::pair<uint16_t, uint16_t>GetStartPosition();

	void HandleInput(GameMap& gameMap);

	void UpdateBullets(GameMap& gameMap);

	std::string GetPositionState() const;

	void IncreaseSpeed(double multiplier);
	void ReduceFireRate(double factor);
	void ActivateShield();
	void updatePowerUps();
	void GainExtraLife();

	void ApplyPowerUpEffect(PowerUpType powerUp);
	void BuyPowerUp(PowerUpType powerUpType);

	bool CanAffordPowerUp(int cost) const;
	void DeductPoints(int cost) { m_points -= cost; }

	std::string GetPowerUpState() const;

	bool HasShield();
	int GetLives();
	double GetBulletSpeed();
	double GetFireRate();
	int GetPoints();
	void setPoints(int points);
	void setName(const std::string& name);
	bool IsAlive() const;
	void DecreaseHealth(int health);

private:
	std::chrono::steady_clock::time_point m_lastShootTime;
	std::string m_name;
	std::uint16_t m_health;
	int m_score;
	int m_points;
	Weapon m_weapon;
	std::vector<std::unique_ptr<Bullet>> m_bullets;
	std::pair<uint16_t, uint16_t>m_startPosition;

	bool m_shieldActive=false;
	std::chrono::time_point<std::chrono::steady_clock> shieldStartTime;
	std::chrono::time_point<std::chrono::steady_clock> speedBoostStartTime;
	std::queue<PowerUpType> m_powerUpQueue;
	std::string base_url = "http://localhost:18080";

	bool m_fireRatePurchased=false;

};