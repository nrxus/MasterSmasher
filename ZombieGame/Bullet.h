#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <Bengine/SpriteBatch.h>

class Human;
class Zombie;
class Agent;

class Bullet {
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();

	bool update(const std::vector<std::string>& levelData, float deltaTime);
	void draw(Bengine::SpriteBatch& spriteBatch);
	bool collideWithAgent(Agent* agent);

	float getDamage() const { return m_damage; }
	glm::vec2 getPosition() const { return m_position; }

private:
	bool collideWithWorld(const std::vector<std::string>& levelData);

	float m_damage;
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_position;
};
