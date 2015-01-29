#pragma once

#include "Shape.h"
#include <Bengine/SpriteBatch.h>
#include <Bengine/SpriteFont.h>
#include <functional>

class SmasherGame;

class BaseButton {
public:
	BaseButton();
	~BaseButton();

	void draw(Bengine::SpriteBatch& spriteBatch);
	void initialize(const char* text, const char* font, int fontSize, glm::vec2 position,
									std::function<bool(BaseButton&, const glm::vec2&)> hoverFunc,
									std::function<void(SmasherGame&, BaseButton&)> clickFunc,
									const glm::vec2& scale = glm::vec2(1.0),
									Bengine::ColorRGBA8 color = Bengine::WHITE_COLOR,
									ShapeType shapeType = ShapeType::RECTANGLE);
	bool hoverFunc(const glm::vec2& mousePos);
	void clickFunc(SmasherGame& game);

	bool contains(const glm::vec2& point) const { return m_shape->contains(point); }
	void setColor(const Bengine::ColorRGBA8& color) { m_color = color; }
	const char* getText() const { return m_buffer; }

private:
	const Bengine::Justification CENTERED = Bengine::Justification::MIDDLE;

	std::function<bool(BaseButton&, const glm::vec2&)> m_hoverFunc;
	std::function<void(SmasherGame&,BaseButton&)> m_clickFunc;
	Shape* m_shape = nullptr;
	Bengine::SpriteFont* m_spriteFont = nullptr;
	Bengine::ColorRGBA8 m_color;
	char m_buffer[256];
	glm::vec2 m_position;
	glm::vec2 m_scale;
};
