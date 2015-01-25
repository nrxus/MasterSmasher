#include "Zombie.h"

#include "Human.h"
#include "ResourceManager.h"

Zombie::Zombie() {}

Zombie::~Zombie() {}

void Zombie::init(float speed, glm::vec2 pos) {
	m_speed = speed;
	m_position = pos;
	m_health = 150;
	m_color = Bengine::ColorRGBA8 (255, 255, 255, 255);
	m_textureID = Bengine::ResourceManager::getTexture("Textures/zombie.png").id;
}

void Zombie::update(const std::vector<std::string>& levelData,
										std::vector<Human*> humans,
										std::vector<Zombie*> zombies,
										float deltaTime) {
	Human* closestHuman = getNearestHuman(humans);
	if (closestHuman != nullptr) {
		m_direction = glm::normalize(closestHuman->getPosition() - m_position);
		m_position += m_direction * m_speed * deltaTime;
	}
	collideWithLevel(levelData);
}

Human* Zombie::getNearestHuman(std::vector<Human*> humans) {
	Human* closestHuman = nullptr;
	float smallestDistance = 9999999.0f;
	for (size_t i = 0; i < humans.size(); i++) {
		glm::vec2 distVec = humans[i]->getPosition() - m_position;
		float distance = glm::length(distVec);
		if(distance < smallestDistance) {
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}
	return closestHuman;
}
