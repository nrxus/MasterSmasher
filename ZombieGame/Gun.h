#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <Bengine/AudioEngine.h>

class Bullet;

class Gun {
public:
	Gun(std::string name, int fireRate, int bulletsPerShot,
			float spread, float bulletDamage, float bulletSpeed, Bengine::SoundEffect fireEffect);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime);

private:
	void fire(const glm::vec2& position, const glm::vec2 direction, std::vector<Bullet>& bullets);

	float m_frameCounter = 0.0f;

	size_t m_bulletsPerShot; //at a time
	int m_fireRate; //per Frames
	float m_bulletDamage;
	float m_bulletSpeed;
	float m_spread; //Accuracy
	std::string m_name;

	Bengine::SoundEffect m_fireEffect;
};
