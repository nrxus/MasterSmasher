#pragma once

#include <Bengine/GLTexture.h>
#include <Bengine/SpriteBatch.h>

#include "Shape.h"
#include "Animation.h"

#include <glm/glm.hpp>

class BaseObject {
public:
	BaseObject();
	~BaseObject();

	float distance(const BaseObject& object) const;

	void draw(Bengine::SpriteBatch& spriteBatch, float rotation = 0);
	void initialize(Bengine::GLTexture texture, ShapeType shapeType,
									glm::vec2 position, size_t totalFrames = 1, Uint32 frameTicks = 0,
									bool loop = true, Bengine::ColorRGBA8 color = Bengine::WHITE_COLOR);
	void move(const glm::vec2& center); // Move to
	void scale(const glm::vec2& scale); // For circle, it will take min(scale.x, scale.y)
	void update();

	bool isActive() { return m_animation.isActive(); }

private:
	bool m_needsUpdate;
	float m_frameHeight;
	float m_frameWidth;
	Animation m_animation;
	Shape* m_shape = nullptr;
};
