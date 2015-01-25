#include "Bullet.h"
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include "Level.h"
#include <Bengine/ResourceManager.h>

const int BULLET_RADIUS = 5;

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed) :
	m_damage(damage), m_speed(speed),
	m_direction(direction), m_position(position) {}

Bullet::~Bullet() {}

bool Bullet::update(const std::vector<std::string>& levelData, float deltaTime) {
	m_position += m_direction * m_speed * deltaTime;
	return collideWithWorld(levelData);
}

bool Bullet::collideWithAgent(Agent* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;
	glm::vec2 centerPosA = m_position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPosA - centerPosB;
	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0) {
		return true;
	}
	return false;
}

void Bullet::draw(Bengine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect(m_position.x - BULLET_RADIUS, m_position.y - BULLET_RADIUS, BULLET_RADIUS*2, BULLET_RADIUS*2);
	const glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
	Bengine::ColorRGBA8 color(75,75,75,255);
	spriteBatch.draw(destRect,uvRect, Bengine::ResourceManager::getTexture("Textures/circle.png").id,0.0f,color);
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData) {
	glm::ivec2 gridPosition;
	float xoffset,yoffset;
	if (m_direction.x < 0) {
		xoffset = - BULLET_RADIUS;
	} else  {
		xoffset = BULLET_RADIUS;
	}
	if (m_direction.y < 0) {
		yoffset = - BULLET_RADIUS;
	} else {
		yoffset = BULLET_RADIUS;
	}
	gridPosition.x = floor((m_position.x+xoffset) / (float) TILE_WIDTH);
	gridPosition.y = floor((m_position.y+yoffset) / (float) TILE_WIDTH);

	if(gridPosition.x < 0 || gridPosition.x >= levelData[0].size() ||
		 gridPosition.y < 0 || gridPosition.y >= levelData.size()) {
		return true;
	}
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}
