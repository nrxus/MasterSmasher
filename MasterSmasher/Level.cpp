#include "Level.h"

#include <Bengine/BengineErrors.h>
#include <Bengine/ResourceManager.h>

#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>

Level::Level() {}

Level::~Level() {}

void Level::initialize(const char* name) {
  //Get ID
  char substr[3];
  strncpy(substr, name+6, strlen(name) - 6);
  m_id = atoi(substr);
  loadLevel();
}

//This is ugly as fuck. Feel free to improve it
void Level::loadLevel() {
  std::string filePath = "Levels/Level" + std::to_string(m_id) + ".txt";
  std::ifstream file;
  file.open(filePath);
  if (file.fail()) {
    Bengine::fatalError("failed to open " + filePath);
  }
  std::string line;
  while(std::getline(file,line)) {
    std::stringstream ss(line);
    std::string in;
    std::string sNum, posX, posY;
    size_t num;
    ss >> in;
    if (in == "m") {
      std::getline(file,line);
      ss.clear();
      ss.str(line);
      ss >> posX;
      ss >> posY;
      std::cout << "METEOR @ " << posX << " " << posY << "\n";
      continue;
    }
    ss >> sNum;;
    num = std::stoi(sNum);
    if (in == "#") {
      std::cout << "SHOTS: " << num << "\n";
      continue;
    }
    if (in == "s") {
      m_stars.resize(num);
      for (size_t i = 0; i < num; i++) {
        std::getline(file,line);
        ss.clear();
        ss.str(line);
        ss >> posX;
        ss >> posY;
				m_stars[i].initialize(Bengine::ResourceManager::getTexture("Collidables/Star.png"),
															ShapeType::RECTANGLE,
															glm::vec2(std::stoi(posX),std::stoi(posY)), 2, 150);
      }
      continue;
    }
    if (in == "e") {
      m_enemies.resize(num);
      for (size_t i =0; i < num; i++) {
        std::getline(file,line);
        ss.clear();
        ss.str(line);
        ss >> posX;
        ss >> posY;
        m_enemies[i].initialize(Bengine::ResourceManager::getTexture("Collidables/Spaceship.png"),
																ShapeType::RECTANGLE,
																glm::vec2(std::stoi(posX),std::stoi(posY)), 2, 150);
      }
      continue;
    }
    if (in == "p") {
      std::string radius, strength, flag;
      m_planets.resize(num);
      for (size_t i = 0; i < num; i++) {
				std::getline(file,line);
				ss.clear();
				ss.str(line);
        ss >> posX;
        ss >> posY;
        ss >> radius;
				if (ss.fail()) {
          std::cout << "PLANET @ " << posX << " " << posY << "\n";
          continue;
        }
				ss >> strength;
				ss >> flag;
        std::cout << "PLANET @ " << posX << " " << posY << " " << flag << " " << radius << " " << strength << "\n";
      }
      continue;
    }
  }
}

void Level::draw(Bengine::SpriteBatch& spriteBatch) {
	for (size_t i = 0; i < m_stars.size(); i++) {
    m_stars[i].draw(spriteBatch);
	}
	for (size_t i = 0; i < m_enemies.size(); i++) {
    m_enemies[i].draw(spriteBatch);
	}
}
void Level::update() {
	for (size_t i = 0; i < m_stars.size(); i++) {
		m_stars[i].update();
	}
	for (size_t i = 0; i < m_enemies.size(); i++) {
    m_enemies[i].update();
	}
}
