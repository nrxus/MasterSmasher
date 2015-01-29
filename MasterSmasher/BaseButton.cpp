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
														std::function<bool(BaseButton&, const glm::vec2&)> hoverFunc,
														std::function<void(SmasherGame&, BaseButton&)> clickFunc,
														const glm::vec2& scale,
														Bengine::ColorRGBA8 color, ShapeType shapeType) {
	sprintf(m_buffer, "%s", text);
	m_spriteFont = new Bengine::SpriteFont(font, fontSize);
	m_position = position;
	m_color = color;
	m_scale = scale;
	m_hoverFunc = hoverFunc;
	m_clickFunc = clickFunc;

	glm::vec2 fontDims = m_spriteFont->measure(m_buffer);
	fontDims.y -= fontSize/8.0f;
	glm::vec2 fontCenter(position.x, position.y + fontDims.y / 2.0f);

	if (shapeType == ShapeType::RECTANGLE) {
		m_shape = new Rectangle(fontCenter, fontDims);
	} else {
		m_shape = new Circle(fontCenter, std::min(fontDims.x, fontDims.y) / 2.0f);
	}
}

bool BaseButton::hoverFunc(const glm::vec2& mousePos) {
	return m_hoverFunc(*this, mousePos);
}

void BaseButton::clickFunc(SmasherGame& game) {
	return m_clickFunc(game, *this);
}
