#include "Rectangle.h"
#include "Circle.h"

Rectangle::Rectangle() {}

Rectangle::Rectangle(glm::vec2 center, glm::vec2 dimensions) :
	m_halfDims(dimensions.x / 2.0f, dimensions.y / 2.0f ) {
	m_center = center;
}

Rectangle::~Rectangle() {}

bool Rectangle::contains(const glm::vec2& point) const {
	if (m_center.x - m_halfDims.x > point.x) return false;
	if (m_center.x + m_halfDims.x < point.x) return false;
	if (m_center.y - m_halfDims.y > point.y) return false;
	if (m_center.y + m_halfDims.y < point.y) return false;
	return true;
}

bool Rectangle::intersects(const Rectangle& r) const {
	if (m_center.x - m_halfDims.x > r.m_center.x + r.m_halfDims.x) return false;
	if (m_center.x + m_halfDims.x < r.m_center.x - r.m_halfDims.x) return false;
	if (m_center.y - m_halfDims.y > r.m_center.y + r.m_halfDims.y) return false;
	if (m_center.y + m_halfDims.y < r.m_center.y - r.m_halfDims.y) return false;
	return true;
}

bool Rectangle::intersects(const Circle& c) const {
	return c.intersects(*this);
}

void Rectangle::setDimensions(glm::vec2 dims) {
	m_halfDims.x = dims.x / 2.0f;
	m_halfDims.y = dims.y / 2.0f;
}

glm::vec4 Rectangle::getDestRect() const {
	return glm::vec4(m_center.x - m_halfDims.x, m_center.y - m_halfDims.y,
									 m_halfDims.x * 2.0f, m_halfDims.y * 2.0f);
}
