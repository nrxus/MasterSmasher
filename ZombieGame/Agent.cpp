#include "Agent.h"
#include "Level.h"

#include <Bengine/ResourceManager.h>

#include <algorithm>

Agent::Agent() {}

Agent::~Agent() {}

void Agent::draw(Bengine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
	glm::vec4 destRect;
	destRect.x = m_position.x; destRect.y = m_position.y;
	destRect.w = AGENT_WIDTH; destRect.z = AGENT_WIDTH;
	spriteBatch.draw(destRect,uvRect,m_textureID,0.0f, m_color, m_direction);
}

bool Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePositions;
	checkTilePosition(levelData,
										collideTilePositions,
										m_position.x, m_position.y);
	checkTilePosition(levelData,
										collideTilePositions,
										m_position.x, m_position.y + AGENT_WIDTH);
	checkTilePosition(levelData,
										collideTilePositions,
										m_position.x + AGENT_WIDTH, m_position.y);
	checkTilePosition(levelData,
										collideTilePositions,
										m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH);

	if (collideTilePositions.size()==0) return false;

	for (size_t i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}
	return true;
}

bool Agent::collideWithAgent(Agent* agent) {
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;
	glm::vec2 centerPosA = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPosA - centerPosB;
	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0) {
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		m_position += collisionDepthVec / 2.0f;
		agent->m_position -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}

bool Agent::applyDamage(float damage) {
	m_health -= damage;
	return (m_health <= 0);
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData,
															std::vector<glm::vec2>& collideTilePositions,
															float x, float y) {
	glm::vec2 cornerPos = glm::vec2(floor(x / (float) TILE_WIDTH),
																	floor(y / (float) TILE_WIDTH));
	if(cornerPos.x < 0 || cornerPos.x >= levelData[0].size() ||
		 cornerPos.y < 0 || cornerPos.y >= levelData.size()) {
		return;
	}
	if(levelData[cornerPos.y][cornerPos.x] != '.') {
		collideTilePositions.push_back(cornerPos * (float) TILE_WIDTH + glm::vec2((float)TILE_WIDTH/2.0f));
	}
}

void Agent::collideWithTile(glm::vec2 tilePos) {
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	glm::vec2 centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos;

	float xDepth = MIN_DISTANCE -	abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if(xDepth > 0 && yDepth > 0) {
		if (std::max(xDepth,0.0f) < std::max(yDepth, 0.0f)) {
			if (distVec.x < 0) {
				m_position.x -= xDepth;
			} else {
				m_position.x += xDepth;
			}
		} else {
			if (distVec.y < 0) {
				m_position.y -= yDepth;
			} else {
				m_position.y += yDepth;
			}
		}
	}
}
