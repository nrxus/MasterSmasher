#include "Gun.h"
#include "Bullet.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>

Gun::Gun(std::string name, int fireRate, int bulletsPerShot,
				 float spread, float bulletDamage, float bulletSpeed, Bengine::SoundEffect fireEffect) :
	m_bulletsPerShot(bulletsPerShot), m_fireRate(fireRate), m_bulletDamage(bulletDamage),
	m_bulletSpeed(bulletSpeed), m_spread(spread), m_name(name), m_fireEffect(fireEffect) {}

Gun::~Gun() {}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets, float deltaTime) {
	m_frameCounter+= 1.0f * deltaTime;
	if (m_frameCounter >= m_fireRate && isMouseDown) {
		fire(position,direction, bullets);
		m_frameCounter=0;
	}
}

void Gun::fire(const glm::vec2& position, const glm::vec2 direction, std::vector<Bullet>& bullets) {
	static std::mt19937 randomEngine2(time(nullptr));
	std::uniform_real_distribution<float> randRotate(-m_spread, m_spread);

	m_fireEffect.play();

	for (size_t i = 0; i < m_bulletsPerShot; i++) {
		bullets.emplace_back(position,
												 glm::rotate(direction,randRotate(randomEngine2)),
												 m_bulletDamage, m_bulletSpeed);
	}

}
