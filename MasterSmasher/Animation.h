#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include <Bengine/SpriteBatch.h>
#include <Bengine/Vertex.h>
#include <Bengine/GLTexture.h>

//TODO: If the time between updates is longer than frameTicks, there might be an issue with frames not updating fast enough. Should not be an issue for now but it is worth looking at

class Animation {
public:
	Animation();
	~Animation();

	void initialize(GLuint texture, size_t totalFrames, Uint32 frameTicks,
									glm::vec4 destRect, bool loop = true,
									Bengine::ColorRGBA8 color = Bengine::WHITE_COLOR);
	void draw(Bengine::SpriteBatch& spriteBatch, float rotation = 0);
	void update();

	void setDestRect(const glm::vec4& destRect);
	bool isActive() const { return m_active; }

private:
	bool m_active;
	bool m_loop;

	int m_currentFrame;
	int m_totalFrames;

	Uint32 m_previousTicks;
	Uint32 m_frameTicks;
	Uint32 m_elapsedTicks;

	glm::vec4 m_destRect;
	glm::vec4 m_uvRect;

	Bengine::ColorRGBA8 m_color;
	GLuint m_texture;
};
