#include "BaseObject.h"

#include "Circle.h"
#include "Rectangle.h"

#include <algorithm>

#include <iostream>

BaseObject::BaseObject() {}

BaseObject::~BaseObject() {
	delete m_shape;
}

float BaseObject::distance(const BaseObject& object) const {
	return m_shape->distance(*(object.m_shape));
}

void BaseObject::draw(Bengine::SpriteBatch& spriteBatch, float rotation) {
	m_animation.draw(spriteBatch,rotation);
}

void BaseObject::initialize(Bengine::GLTexture texture, ShapeType shapeType,
														glm::vec2 position, size_t totalFrames, Uint32 frameTicks,
														bool loop, Bengine::ColorRGBA8 color) {
	m_frameHeight = ((float) texture.height) / totalFrames;
	m_frameWidth = ((float) texture.width) / totalFrames;

	if (shapeType == ShapeType::RECTANGLE) {
		m_shape = new Rectangle(position, glm::vec2(m_frameWidth, m_frameHeight));
	} else {
		m_shape = new Circle(position, std::min(m_frameWidth, m_frameHeight) / 2.0f);
	}
	m_animation.initialize(texture.id, totalFrames, frameTicks,
												 m_shape->getDestRect(), loop, color);
	std::cout << m_shape->getDestRect().z << " " << m_shape->getDestRect().w << std::endl;
}

void BaseObject::move(const glm::vec2& center) {
	m_needsUpdate = true;
	m_shape->setCenter(center);
}

void BaseObject::scale(const glm::vec2& scale) {
	m_needsUpdate = true;
	m_shape->setDimensions(glm::vec2(m_frameWidth * scale.x, m_frameHeight * scale.y));
}

void BaseObject::update() {
	if (!m_animation.isActive()) return;
	if (m_needsUpdate) {
		m_animation.setDestRect(m_shape->getDestRect());
		m_needsUpdate = false;
	}
	m_animation.update();
}
