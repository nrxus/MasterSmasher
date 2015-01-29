#pragma once

#include <stdio.h>
#include <vector>

#include "BaseObject.h"
#include <Bengine/SpriteBatch.h>

class Level {
public:
	Level();
	~Level();

	void initialize(const char* name);
	void draw(Bengine::SpriteBatch& spriteBatch);
	void update();

private:
	void loadLevel();

	size_t m_id = 0;
	size_t m_shots = 0;
	std::vector<BaseObject> m_stars;
	std::vector<BaseObject> m_planets;
	std::vector<BaseObject> m_enemies;
	BaseObject m_meteor;
};
