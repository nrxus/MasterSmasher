#include "BaseButton.h"

#include "Circle.h"
#include "Rectangle.h"

#include <algorithm>

BaseButton::BaseButton() {}

BaseButton::~BaseButton() {}

void BaseButton::draw(Bengine::SpriteBatch& spriteBatch) {
	m_spriteFont->draw(spriteBatch, m_buffer, m_position, m_scale,
										 0.0f, m_color, CENTERED);
}

void BaseButton::initialize(const char* text, const char* font, int fontSize,
														glm::vec2 position,
														std::function<bool(SmasherGame&, BaseButton&)> updateFunc,
														const glm::vec2& scale,
														Bengine::ColorRGBA8 color, ShapeType shapeType) {
	sprintf(m_buffer, "%s", text);
	m_spriteFont = new Bengine::SpriteFont(font, fontSize);
	m_position = position;
	m_color = color;
	m_scale = scale;
	m_updateFunc = updateFunc;

	glm::vec2 fontDims = m_spriteFont->measure(m_buffer);
	fontDims.y -= fontSize/8.0f;
	glm::vec2 fontCenter(position.x, position.y + fontDims.y / 2.0f);

	if (shapeType == ShapeType::RECTANGLE) {
		m_shape = new Rectangle(fontCenter, fontDims);
	} else {
		m_shape = new Circle(fontCenter, std::min(fontDims.x, fontDims.y) / 2.0f);
	}
}

bool BaseButton::update(SmasherGame& game) {
	return m_updateFunc(game,*this);
}
