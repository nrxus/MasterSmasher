#pragma once

#include <string>
#include <vector>
#include <Bengine/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level {
public:
  // Load the level
  Level(const std::string& fileName);
  ~Level();

  void draw();

  glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
  const std::vector<glm::vec2>& getZombieStartPositions() const { return m_zombieStartPosition; }
  const std::vector<std::string>& getLevelData() const { return m_levelData; }
  int getNumHumans() const { return m_numHumans; }
  int getWidth() const { return m_levelData[0].size(); }
  int getHeight() const { return m_levelData.size(); }

private:
  int m_numHumans;
  glm::vec2 m_startPlayerPos;
  std::vector<std::string> m_levelData;
  std::vector<glm::vec2> m_zombieStartPosition;
  Bengine::SpriteBatch m_spriteBatch;
};
