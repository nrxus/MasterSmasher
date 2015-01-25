#pragma once

#include "Shape.h"

class Circle: public Shape {
public:
	Circle();
	Circle(glm::vec2 center, float radius);
	virtual ~Circle();

	bool contains(const glm::vec2& point) const;
	bool intersects(const Rectangle& r) const;
	bool intersects(const Circle& c) const;
	bool intersects(const glm::vec2& p1, const glm::vec2& p2) const;
	void setDimensions(glm::vec2 dims);

	glm::vec4 getDestRect() const;

	float getRadius() const { return m_radius; }

protected:
	float m_radius;
};
