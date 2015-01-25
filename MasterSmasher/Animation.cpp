#include "Animation.h"

Animation::Animation() {}

Animation::~Animation() {}

void Animation::initialize(GLuint texture, size_t totalFrames, Uint32 frameTicks,
													 glm::vec4 destRect, bool loop, Bengine::ColorRGBA8 color) {
	m_texture = texture;
	m_totalFrames = totalFrames;
	m_frameTicks = frameTicks;
	m_loop = loop;
	m_color = color;
	m_destRect = destRect;

	m_active = true;
	m_currentFrame = 0;
	m_previousTicks = 0;
	m_elapsedTicks = 0;
	m_uvRect = glm::vec4(0.0f, 0.0f, 1.0f/m_totalFrames, 1.0f);
}

void Animation::draw(Bengine::SpriteBatch& spriteBatch, float rotation) {
	if (!m_active) return;
	if (rotation == 0) {
		spriteBatch.draw(m_destRect,m_uvRect,m_texture,0.0f,m_color);
		return;
	}
	spriteBatch.draw(m_destRect,m_uvRect,m_texture,0.0f,m_color,rotation);
}

void Animation::update() {
	if (!m_active) return;
	Uint32 currentTicks = SDL_GetTicks();
	m_elapsedTicks += currentTicks - m_previousTicks;
	if (m_elapsedTicks > m_frameTicks) {
		m_currentFrame = (m_currentFrame + 1) % (m_totalFrames);
		if (m_currentFrame == 0 && !m_loop) m_active = false;
		m_elapsedTicks -= m_frameTicks;
		float startFrame = m_currentFrame / (float) m_totalFrames;
		m_uvRect = glm::vec4(startFrame, 0.0f, 1.0f / m_totalFrames, 1.0f);
	}
	m_previousTicks = currentTicks;
}

void Animation::setDestRect(const glm::vec4& destRect) {
	if (destRect.x < - destRect.z) {
		m_active = false;
		return;
	}
	m_destRect = destRect;
}
