#include "Animation.h"

Animation::Animation() {}

Animation::~Animation() {}

void Animation::Draw(Bengine::SpriteBatch spriteBatch, float rotation) {
	if (!m_active) return; //Ideally we should not even be calling for draw.
	if (rotation==0) {
		spriteBatch.draw(*m_destRect,m_uvRect,m_texture.id,0.0f,m_color);
		return;
	}
	spriteBatch.draw(*m_destRect,m_uvRect,m_texture.id,0.0f,m_color,rotation);
}
