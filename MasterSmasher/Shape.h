#pragma once

#include <glm/glm.hpp>

class Rectangle;
class Circle;

class Shape {
public:
	Shape() {}
	virtual ~Shape() {}
	virtual bool contains(const glm::vec2& point) const = 0;
	virtual bool intersects(const Rectangle& r) const = 0;
	virtual bool intersects(const Circle& c) const = 0;

	virtual void setDimensions(glm::vec2 dims) = 0;

	virtual glm::vec4 getDestRect() const = 0;

	float distance(const Shape& shape) { return glm::length(m_center - shape.m_center); }
	void setCenter(const glm::vec2& center) { m_center = center; }
	glm::vec2 getCenter() const { return m_center; }

protected:
	glm::vec2 m_center = glm::vec2(0.0f);
	float angle = 0; //TODO: Implement this in collision detection
};
