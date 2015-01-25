#include "ZombieGame.h"

#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>
#include <Bengine/BengineErrors.h>
#include <Bengine/ResourceManager.h>

#include <SDL2/SDL.h>
#include <glm/gtx/rotate_vector.hpp>
#include <random>
#include <ctime>
#include <algorithm>

#include "Zombie.h"
#include "Gun.h"

const float HUMAN_SPEED = 1.0f;
const float PLAYER_SPEED = 5.0f;
const float ZOMBIE_SPEED = 1.3f;

ZombieGame::ZombieGame() {}

ZombieGame::~ZombieGame() {
  for (size_t i = 0; i < m_levels.size(); i++) {
    delete m_levels[i];
  }
  for (size_t i = 0; i < m_humans.size(); i++) {
    delete m_humans[i];
  }
  for (size_t i = 0; i < m_zombies.size(); i++) {
    delete m_zombies[i];
  }
}

void ZombieGame::run() {
  initSystems();
  initLevel();
  Bengine::Music music = m_audioEngine.loadMusic("Sound/XYZ.ogg");
  music.play(-1);
  gameLoop();
}

void ZombieGame::initSystems() {
  Bengine::init();
  m_audioEngine.init();
  m_window.create("ZombieGame", m_screenWidth, m_screenHeight, 0);
  glClearColor(0.7f,0.7f,0.7f,1.0f);
  initShaders();

  m_agentSpriteBatch.init();
  m_hudSpriteBatch.init();

  m_spriteFont = new Bengine::SpriteFont("Fonts/chintzy.ttf",64);

  m_camera.init(m_screenWidth,m_screenHeight);
  m_hudCamera.init(m_screenWidth,m_screenHeight);
  const float CAMERA_SCALE = 1.0f/3.0f;
  m_camera.setScale(CAMERA_SCALE);
  m_hudCamera.setPosition(glm::vec2(m_screenWidth/2, m_screenHeight/2));

  m_bloodParticleBatch = new Bengine::ParticleBatch2D();
  m_bloodParticleBatch->init(1000,
                             0.05f,
                             Bengine::ResourceManager::getTexture("Textures/particle.png"),
                             [](Bengine::Particle2D& particle, float deltaTime) {
                               particle.position += particle.velocity * deltaTime;
                               particle.color.a = (GLubyte) (particle.life*255.0f);
                             });
  m_particleEngine.addParticleBatch(m_bloodParticleBatch);
}

void ZombieGame::initLevel () {
  // Level 1
  m_levels.push_back(new Level("Levels/level1.txt"));
  m_currentLevel=0;

  m_player = new Player();
  m_player->init(PLAYER_SPEED,m_levels[m_currentLevel]->getStartPlayerPos(),&m_inputManager, &m_camera,&m_bullets);

  m_humans.push_back(m_player);

  std::mt19937 rEngine(time(nullptr));
  std::uniform_int_distribution<int> randX(2,m_levels[m_currentLevel]->getWidth() - 2);
  std::uniform_int_distribution<int> randY(2,m_levels[m_currentLevel]->getHeight() - 2);

  for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++) {
    m_humans.push_back(new Human);
    glm::vec2 pos(randX(rEngine) * TILE_WIDTH,randY(rEngine) * TILE_WIDTH);
    m_humans.back()->init(HUMAN_SPEED,pos);
  }

  const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getZombieStartPositions();
  for (size_t i = 0; i < zombiePositions.size(); i++) {
    m_zombies.push_back(new Zombie);
    m_zombies.back()->init(ZOMBIE_SPEED,zombiePositions[i]);
  }
  m_player->addGun(new Gun("Magnum",10,1,glm::radians(5.0f),30,20.0f,m_audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
  m_player->addGun(new Gun("Shotgun",30,12,glm::radians(20.0f),4,20.0f,m_audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
  m_player->addGun(new Gun("MP5",2,1,glm::radians(10.0f),20,20.0f,m_audioEngine.loadSoundEffect("Sound/shots/rifle.wav")));
}

void ZombieGame::initShaders() {
  // Compile our color shader
  m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
  m_textureProgram.addAttribute("vertexPosition");
  m_textureProgram.addAttribute("vertexColor");
  m_textureProgram.addAttribute("vertexUV");
  m_textureProgram.linkShaders();
}

void ZombieGame::gameLoop() {
  Bengine::FpsLimiter fpsLimiter;
  const int MAX_PHYSICS_STEPS = 6;
  const float DESIRED_FPS = 60.0f;
  const float MS_PER_SECOND = 1000;
  const float DESIRED_FRAMETIME = MS_PER_SECOND/DESIRED_FPS;
  const float MAX_DELTA_TIME = 1.0f;
  fpsLimiter.setMaxFPS(DESIRED_FPS);
  float previousTicks = SDL_GetTicks();

  while(m_gameState == GameState::PLAY) {
    fpsLimiter.begin();
    float newTicks = SDL_GetTicks();
    float frameTime = newTicks - previousTicks;
    previousTicks = newTicks;
    float totalDeltaTime = frameTime / DESIRED_FRAMETIME;
    checkVictory();
    m_inputManager.update();
    processInput();
    int i = 0;
    while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
      float deltaTime = std::min(totalDeltaTime,MAX_DELTA_TIME);
      updateAgents(deltaTime);
      updateBullets(deltaTime);
      m_particleEngine.update(deltaTime);
      totalDeltaTime -= deltaTime;
      i++;
    }

    m_camera.setPosition(m_player->getPosition());
    m_camera.update();
    m_hudCamera.update();
    drawGame();
    m_fps = fpsLimiter.end();
  }
}

void ZombieGame::updateAgents(float deltaTime) {
  //Update Humans
  for (size_t i = 0; i < m_humans.size(); i++) {
    m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(),
                        m_humans,
                        m_zombies,
                        deltaTime);
  }
  //Update Zombies
  for (size_t i = 0; i < m_zombies.size(); i++) {
    m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(),
                         m_humans,
                         m_zombies,
                         deltaTime);
  }
  //Zombies Collide
  for (size_t i = 0; i < m_zombies.size(); i++) {
    //against other zombies
    for (size_t j = i + 1; j < m_zombies.size(); j++) {
      m_zombies[i]->collideWithAgent(m_zombies[j]);
    }
    //against NPC Humans
    for (size_t j = 1; j < m_humans.size(); j++) {
      if (m_zombies[i]->collideWithAgent(m_humans[j])) {
        m_zombies.push_back(new Zombie);
        m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition());
        delete m_humans[j];
        m_humans[j] = m_humans.back();
        m_humans.pop_back();
      }
    }
    //against player
    if (m_zombies[i]->collideWithAgent(m_player)) {
      Bengine::fatalError("YOU LOSE!");
    }
  }

  for (size_t i = 0; i < m_humans.size(); i++) {
    for (size_t j = i + 1; j < m_humans.size(); j++) {
      m_humans[i]->collideWithAgent(m_humans[j]);
    }
  }
}

void ZombieGame::updateBullets(float deltaTime) {
  for (size_t i = 0; i < m_bullets.size();) {
    if(m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(),deltaTime)) {
      m_bullets[i] = m_bullets.back();
      m_bullets.pop_back();
    }
    else {
      i++;
    }
  }

  for (size_t i = 0; i < m_bullets.size(); i++) {
    bool bulletHit = false;
    //Zombie damage
    for (size_t j = 0; j < m_zombies.size(); j++) {
      if(m_bullets[i].collideWithAgent(m_zombies[j])) {
        addBlood(m_bullets[i].getPosition(), 5);
        if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {
          delete m_zombies[j];
          m_zombies[j] = m_zombies.back();
          m_zombies.pop_back();
          m_numZombiesKilled++;
        }
        m_bullets[i] = m_bullets.back();
        m_bullets.pop_back();
        bulletHit = true;
        break;
      }
    }
    if (bulletHit) continue;
    //Human damage
    for (size_t j = 1; j < m_humans.size(); j++) {
      if(m_bullets[i].collideWithAgent(m_humans[j])) {
        addBlood(m_bullets[i].getPosition(), 5);
        if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
          delete m_humans[j];
          m_humans[j] = m_humans.back();
          m_humans.pop_back();
          m_numHumansKilled++;
        }
        m_bullets[i] = m_bullets.back();
        m_bullets.pop_back();
        bulletHit = true;
        break;
      }
    }
    if(bulletHit) continue;
    i++;
  }

}

void ZombieGame::checkVictory() {
  //TODO: Support for multiple levels
  if (m_zombies.empty()) {
    std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %lu/%d civilians remaining",m_numHumansKilled,m_numZombiesKilled, m_humans.size() - 1,m_levels[m_currentLevel]->getNumHumans());
    Bengine::fatalError("");
  }
}

void ZombieGame::processInput() {
  SDL_Event evnt;
  //Will keep looping until there are no more events to process
  while (SDL_PollEvent(&evnt)) {
    switch (evnt.type) {
    case SDL_QUIT:
      m_gameState = GameState::EXIT;
      break;
    case SDL_MOUSEMOTION:
      m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
      break;
    case SDL_KEYDOWN:
      m_inputManager.pressKey(evnt.key.keysym.sym);
      break;
    case SDL_KEYUP:
      m_inputManager.releaseKey(evnt.key.keysym.sym);
      break;
    case SDL_MOUSEBUTTONDOWN:
      m_inputManager.pressKey(evnt.button.button);
      break;
    case SDL_MOUSEBUTTONUP:
      m_inputManager.releaseKey(evnt.button.button);
      break;
    }
  }
}

void ZombieGame::drawGame() {
  // Set the base depth to 1.0
  glClearDepth(1.0);
  // Clear the color and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_textureProgram.use();

  glActiveTexture(GL_TEXTURE0);
  GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
  glUniform1i(textureUniform, 0);

  glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
  GLint pUniform = m_textureProgram.getUniformLocation("P");
  glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

  m_levels[m_currentLevel]->draw();

  const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

  m_agentSpriteBatch.begin();
  for (size_t i = 0; i < m_humans.size(); i++) {
    if (m_camera.isBoxInView(m_humans[i]->getPosition(),agentDims)) {
      m_humans[i]->draw(m_agentSpriteBatch);
    }
  }
  for (size_t i = 0; i < m_zombies.size(); i++) {
    if (m_camera.isBoxInView(m_zombies[i]->getPosition(),agentDims)) {
      m_zombies[i]->draw(m_agentSpriteBatch);
    }
  }
  for (size_t i = 0; i < m_bullets.size(); i++) {
    m_bullets[i].draw(m_agentSpriteBatch);
  }
  m_agentSpriteBatch.end();
  m_agentSpriteBatch.renderBatch();
  m_particleEngine.draw(&m_agentSpriteBatch);
  drawHUD();


  m_textureProgram.unuse();
  // Swap our buffer and draw everything to the screen!
  m_window.swapBuffer();
}

void ZombieGame::drawHUD() {
  char buffer[256];
  glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
  GLint pUniform = m_textureProgram.getUniformLocation("P");
  glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
  m_hudSpriteBatch.begin();
  sprintf(buffer, "Num Humans: %lu", m_humans.size());
  m_spriteFont->draw(m_hudSpriteBatch, buffer,
                     glm::vec2(0,0),glm::vec2(0.5),0.0f,
                     Bengine::ColorRGBA8(255,255,255,255));
  sprintf(buffer, "Num Zombies: %lu", m_zombies.size());
  m_spriteFont->draw(m_hudSpriteBatch, buffer,
                     glm::vec2(0,36),glm::vec2(0.5),0.0f,
                     Bengine::ColorRGBA8(255,255,255,255));
  m_hudSpriteBatch.end();
  m_hudSpriteBatch.renderBatch();
}

void ZombieGame::addBlood(const glm::vec2& position, int numParticles) {
  static std::mt19937 randEngine(time(nullptr));
  static std::uniform_real_distribution<float> randAngle(0.0f, 2.0f * M_PI);
  glm::vec2 vel(2.0f, 0.0f);
  Bengine::ColorRGBA8 col(255, 0, 0, 255);
  for (int i = 0; i < numParticles; i++) {
    m_bloodParticleBatch->addParticle(position,glm::rotate(vel, randAngle(randEngine)),
                                      col, 30.0f);
  }
}
