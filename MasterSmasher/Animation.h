#pragma once

#include <glm/glm.hpp>

#include <Bengine/SpriteBatch.h>
#include <Bengine/Vertex.h>
#include <Bengine/GLTexture.h>

class Animation {
public:
	Animation();
	~Animation();

	void initialize(Bengine::GLTexture texture, glm::vec4* destRect, int frameCount,
									int frameTicks, bool loop = true,
									Bengine::ColorRGBA8 color = Bengine::WHITE_COLOR);
	void draw(Bengine::SpriteBatch spriteBatch, float rotation = 0);

private:
	bool m_loop;
	bool m_active;

	glm::vec4 m_uvRect;
	glm::vec4* m_destRect;

	Bengine::ColorRGBA8 m_color;
	Bengine::GLTexture m_texture;
};
