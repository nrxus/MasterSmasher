#pragma once

#include "Human.h"
#include "Bullet.h"
#include <Bengine/InputManager.h>
#include <Bengine/Camera2D.h>

class Gun;

class Player : public Human {
public:
	Player();
	virtual ~Player();

	void init(float speed, glm::vec2 pos, Bengine::InputManager* inputManager, Bengine::Camera2D* camera, std::vector<Bullet>* bullets);

	void addGun(Gun* gun);

	void update(const std::vector<std::string>& levelData,
							std::vector<Human*> humans,
							std::vector<Zombie*> zombies,
							float deltaTime);

private:
	Bengine::InputManager* m_inputManager;
	Bengine::Camera2D* m_camera;
	std::vector<Bullet>* m_bullets;
	std::vector<Gun*> m_guns;
	int m_currentGunIndex;
};