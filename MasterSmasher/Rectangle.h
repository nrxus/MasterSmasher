#pragma once

#include "Shape.h"

class Rectangle : public Shape {
public:
	Rectangle();
	Rectangle(glm::vec2 center, glm::vec2 dimensions);
	virtual ~Rectangle();

	bool contains(const glm::vec2& point) const;
	bool intersects(const Rectangle& r) const;
	bool intersects(const Circle& c) const;
	void setDimensions(glm::vec2 dims);

	glm::vec2 getHalfDims() const { return m_halfDims; }

protected:
	glm::vec2 m_halfDims = glm::vec2(0.0f);
};
