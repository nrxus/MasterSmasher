#include "Human.h"
#include "ResourceManager.h"
#include <random>
#include <ctime>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>

Human::Human() : m_frames(0) {}

Human::~Human() {}

void Human::init(float speed, glm::vec2 pos) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f,1.0f);

	m_health = 20;

	m_speed = speed;
	m_position = pos;
	m_direction = glm::vec2(randDir(randomEngine),randDir(randomEngine));
	// In case the random direction was (0,0)
	if (m_direction.length() == 0) {
		m_direction = glm::vec2(1.0,0.0f);
	}
	m_direction = glm::normalize(m_direction);
	m_color = Bengine::ColorRGBA8 (255, 255, 255, 255);
	m_textureID = Bengine::ResourceManager::getTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData,
									 std::vector<Human*> humans,
									 std::vector<Zombie*> zombies,
									 float deltaTime) {
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-glm::quarter_pi<float>(),glm::quarter_pi<float>());
	m_position += m_direction * m_speed * deltaTime;

	//Randomly change direction every 20 frames
	if(m_frames==20) {
		m_direction = glm::rotate(m_direction,randRotate(randomEngine));
		m_frames=0;
	} else {
		m_frames++;
	}

	if (collideWithLevel(levelData)) {
		m_direction = glm::rotate(m_direction,randRotate(randomEngine));
	}
}
