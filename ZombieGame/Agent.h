#pragma once
#include <glm/glm.hpp>
#include "Bengine/SpriteBatch.h"
#include <string>

const float AGENT_WIDTH = 60.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent {
public:
  Agent();
  virtual ~Agent();

  void draw(Bengine::SpriteBatch& spriteBatch);
  bool collideWithLevel(const std::vector<std::string>& levelData);

  bool collideWithAgent(Agent* agent);
  bool applyDamage(float damage);

  //Getters and Setters
  glm::vec2 getPosition() const { return m_position; }

  //Virtual functions
  virtual void update(const std::vector<std::string>& levelData,
                      std::vector<Human*> humans,
                      std::vector<Zombie*> zombies,
                      float deltaTime) = 0;

protected:
  void checkTilePosition(const std::vector<std::string>& levelData,
                         std::vector<glm::vec2>& collideTilePositions,
                         float x, float y);
  void collideWithTile(glm::vec2 tilePos);

  Bengine::ColorRGBA8 m_color;
  glm::vec2 m_position;
  glm::vec2 m_direction = glm::vec2(1.0f,0.0f);
  float m_speed;
  float m_health;
  GLuint m_textureID;
};