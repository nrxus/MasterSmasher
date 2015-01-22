#include "Circle.h"
#include "Rectangle.h"

#include <iostream>
#include <algorithm>

Circle::Circle() {}

Circle::Circle(glm::vec2 center, float radius) :
	m_radius(radius) {
	m_center = center;
}

Circle::~Circle() {}

bool Circle::contains(const glm::vec2& point) const {
	float distance = glm::length(m_center-point);
	return distance < m_radius;
}

bool Circle::intersects(const Rectangle& r) const {
	//TODO: Test more, few cases I have tried seem to work but is copy-pasta so be careful
	if (this->contains(r.getCenter())) return true; //Square inside circle
	glm::vec2 halfDims = r.getHalfDims();
	glm::vec2 rCenter = r.getCenter();

	glm::vec2 tl(rCenter.x - halfDims.x, rCenter.y - halfDims.y);
	glm::vec2 tr(rCenter.x + halfDims.x, rCenter.y - halfDims.y);
	if (this->intersects(tl,tr)) return true; //Top of rectangle intersects
	glm::vec2 bl(rCenter.x - halfDims.x, rCenter.y + halfDims.y);
	if (this->intersects(tl,bl)) return true; //Left of rectangle intersects
	glm::vec2 br(rCenter.x + halfDims.x, rCenter.y + halfDims.y);
	if (this->intersects(tr,br)) return true; //Right of rectangle intersects
	if (this->intersects(bl,br)) return true; //Bottom of rectangle intersects
	return false;
}

bool Circle::intersects(const Circle& c) const {
	float distance = glm::length(m_center-c.m_center);
	return distance < (m_radius + c.m_radius);
	return false;
}

bool Circle::intersects(const glm::vec2& p1, const glm::vec2& p2) const {
	glm::vec2 d = p2-p1;
	glm::vec2 f = p1-m_center;
	float a = glm::dot(d,d);
	float b = 2 * glm::dot(f,d);
	float c = glm::dot(f,f) - m_radius*m_radius;
	float discriminant = b*b-4*a*c;
	if (discriminant < 0) return false;
	discriminant = sqrt(discriminant);
	float t1 = (-b - discriminant)/(2*a);
  float t2 = (-b + discriminant)/(2*a);

	if(t1 >= 0 && t1 <= 1)  return true;
	if(t2 >= 0 && t2 <= 1)  return true;
	return false;
}

void Circle::setDimensions(glm::vec2 dims) {
	m_radius = std::max(dims.x,dims.y) / 2.0f;
}
