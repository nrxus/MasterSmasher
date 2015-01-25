#include "Level.h"

#include <Bengine/ResourceManager.h>
#include <Bengine/BengineErrors.h>

#include <fstream>

const std::string TX_FOLDER = "Textures/";

Level::Level(const std::string& fileName) {
  std::ifstream file;
  file.open(fileName);
  if (file.fail()) {
    Bengine::fatalError("failed open " + fileName);
  }
  // Throw away the first string in tmp
  std::string tmp;
  file >> tmp >> m_numHumans;
  std::getline(file,tmp); // Throw away
  while(std::getline(file, tmp)) {
    m_levelData.push_back(tmp);
  }
  m_spriteBatch.init();
  m_spriteBatch.begin();
  glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
  Bengine::ColorRGBA8 whiteColor(255, 255, 255, 255);

  for (size_t j = 0; j < m_levelData.size(); j++) {
    for (size_t i = 0; i < m_levelData[j].size(); i++) {
      char tile = m_levelData[j][i];
      glm::vec4 destRect(i*TILE_WIDTH, j*TILE_WIDTH,TILE_WIDTH,TILE_WIDTH);
      switch(tile) {
      case 'B':
      case 'R':
        m_spriteBatch.draw(destRect, uvRect,
                          Bengine::ResourceManager::getTexture(TX_FOLDER+"red_bricks.png").id,
                          0.0f, whiteColor);
        break;
      case 'G':
        m_spriteBatch.draw(destRect, uvRect,
                          Bengine::ResourceManager::getTexture(TX_FOLDER+"glass.png").id,
                          0.0f, whiteColor);
        break;
      case 'L':
        m_spriteBatch.draw(destRect, uvRect,
                          Bengine::ResourceManager::getTexture(TX_FOLDER+"light_bricks.png").id,
                          0.0f, whiteColor);
        break;
      case '@':
        m_levelData[j][i] = '.';
        m_startPlayerPos.x = i * TILE_WIDTH;
        m_startPlayerPos.y = j * TILE_WIDTH;
        break;
      case 'Z':
        m_levelData[j][i] = '.';
        m_zombieStartPosition.emplace_back(i * TILE_WIDTH, j * TILE_WIDTH);
        break;
      case '.':
      case '\r':
        break;
      default:
        std::printf("Unexpected symbol %c at (%lu,%lu)\n",tile,i,j);
      }
    }
  }
  m_spriteBatch.end();
}

Level::~Level() {}

void Level::draw() {
  m_spriteBatch.renderBatch();
}
