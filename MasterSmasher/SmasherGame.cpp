#include "SmasherGame.h"

#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>
#include <Bengine/ResourceManager.h>
#include <Bengine/Vertex.h>

#include "Rectangle.h"
#include "Circle.h"
#include "BaseButton.h"

#include <string>
#include <iostream>
#include <stdio.h>

bool mouseHover(BaseButton& button, const glm::vec2 mousePos) {
  if (button.contains(mousePos)) {
    button.setColor(Bengine::ColorRGBA8(255,255,0,255));
    return true;
  }
  //Mouse over button
  button.setColor(Bengine::WHITE_COLOR);
  return false;
}

void SmasherGame::clickLevelSelect(BaseButton& button) {
  std::string levelName;
  for (size_t i = m_levelButtons.size() + 1; i <= m_maxLevel + 1; i++) {
    levelName = "Level " + std::to_string(i);
    m_levelButtons.emplace_back();
    m_levelButtons[i-1].initialize(levelName.c_str(), "Fonts/kenpixel_mini.ttf", 32,
                                   glm::vec2(m_screenWidth / 2.0f, m_screenHeight / 2.0f),
                                   mouseHover, &SmasherGame::clickLevel);
  }
  m_gameState = GameState::LEVEL_SELECT;
}

void SmasherGame::clickLevel(BaseButton& button) {
  m_level.initialize(button.getText());
  m_gameState = GameState::PLAY;
}

SmasherGame::SmasherGame() {}

SmasherGame::~SmasherGame() {}

void SmasherGame::run() {
  initSystems();
  //Bengine::Music music = m_audioEngine.loadMusic("Music/gameplaySong.ogg");
  //music.play(-1);
  initMainMenu();
  gameLoop();
}

void SmasherGame::drawGame() {
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

  switch(m_gameState) {
  case GameState::MAIN_MENU:
    drawMenu();
    break;
  case GameState::LEVEL_SELECT:
    drawLevelSelect();
    break;
  case GameState::PLAY:
    drawLevel();
  default:
    break;
  }

  if (m_gameState == GameState::MAIN_MENU) drawMenu();

  m_textureProgram.unuse();
  // Swap our buffer and draw everything to the screen!
  m_window.swapBuffer();
}

void SmasherGame::drawMenu() {
  const glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
  glm::vec4 destRect(0.0f, 0.0f, m_screenWidth, m_screenHeight);
  //Draw text
  char buffer[256];
  sprintf(buffer, "master");
  //glm::vec2 txtSizeMaster = m_spriteFontMini->measure("master");
  glm::vec2 txtSizeSmasher = m_spriteFont->measure("SMASHER");
  //std::cout << m_spriteFont->getFontHeight() << std::endl;
  float txtOffset = txtSizeSmasher.y;
  m_spriteBatch.begin(Bengine::GlyphSortType::NONE);
  m_hudSpriteBatch.begin();
  glm::vec2 textPos = glm::vec2(m_screenWidth / 2.0f, 6.0f * m_screenHeight / 8.0f);
  m_spriteFontMini->draw(m_hudSpriteBatch, buffer,
                         textPos,
                         glm::vec2(1.0f), 0.0f, Bengine::WHITE_COLOR,
                         Bengine::Justification::MIDDLE);
  sprintf(buffer, "SMASHER");
  m_spriteFont->draw(m_hudSpriteBatch, buffer,
                     glm::vec2(textPos.x, textPos.y - txtOffset),
                     glm::vec2(1.0f), 0.0f, Bengine::ColorRGBA8(248,184,0,255),
                     Bengine::Justification::MIDDLE);
  //Draw background
  m_spriteBatch.draw(destRect,uvRect,
                     Bengine::ResourceManager::getTexture("Menu/Background.png").id,
                     0.0f,Bengine::WHITE_COLOR);
  //Menu objects
  for (size_t i = 0; i < m_menuButtons.size(); i++) {
    m_menuButtons[i].draw(m_spriteBatch);
  }
  m_spriteBatch.end();
  m_hudSpriteBatch.end();
  m_spriteBatch.renderBatch();
  m_hudSpriteBatch.renderBatch();
}

void SmasherGame::drawLevelSelect() {
  const glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
  glm::vec4 destRect(0.0f, 0.0f, m_screenWidth, m_screenHeight);
  m_spriteBatch.begin(Bengine::GlyphSortType::NONE);
  //Draw background
  m_spriteBatch.draw(destRect,uvRect,
                     Bengine::ResourceManager::getTexture("Menu/Background.png").id,
                     0.0f,Bengine::WHITE_COLOR);
  //Menu objects
  for (size_t i = 0; i < m_menuButtons.size(); i++) {
    m_levelButtons[i].draw(m_spriteBatch);
  }
  m_spriteBatch.end();
  m_spriteBatch.renderBatch();
}

void SmasherGame::drawLevel() {
  const glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
  glm::vec4 destRect(0.0f, 0.0f, m_screenWidth, m_screenHeight);
  m_spriteBatch.begin(Bengine::GlyphSortType::NONE);
  m_spriteBatch.draw(destRect,uvRect,
                     Bengine::ResourceManager::getTexture("Backgrounds/bgGamePlay.png").id,
                     0.0f,Bengine::WHITE_COLOR);
  m_level.draw(m_spriteBatch);
  m_spriteBatch.end();
  m_spriteBatch.renderBatch();
}

void SmasherGame::initMainMenu() {
  m_menuButtons.resize(NUM_MENU_BUTTONS);
  m_levelButtons.reserve(NUM_MAX_LEVELS); //Reserve memory once, avoid re-allocating
  //Select Level Button
  m_menuButtons[0].initialize("Level Select", "Fonts/kenpixel_mini.ttf", 32,
                              glm::vec2(m_screenWidth / 2.0f, 3.0f * m_screenHeight/8.0),
                              mouseHover, &SmasherGame::clickLevelSelect);
}

void SmasherGame::initShaders() {
  // Compile our color shader
  m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
  m_textureProgram.addAttribute("vertexPosition");
  m_textureProgram.addAttribute("vertexColor");
  m_textureProgram.addAttribute("vertexUV");
  m_textureProgram.linkShaders();
}

void SmasherGame::initSystems() {
  Bengine::init();
  m_audioEngine.init();
  m_window.create("MasterSmasher", m_screenWidth, m_screenHeight, 0);
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
  initShaders();

  m_spriteBatch.init();
  m_hudSpriteBatch.init();
  m_spriteFont = new Bengine::SpriteFont("Fonts/kenpixel.ttf",72);
  m_spriteFontMini = new Bengine::SpriteFont("Fonts/kenpixel_mini.ttf",48);
  m_camera.init(m_screenWidth,m_screenHeight);
  m_camera.setPosition(glm::vec2(m_screenWidth / 2.0f,
                                 m_screenHeight / 2.0f)); //Center the camera
}

void SmasherGame::gameLoop() {
  Bengine::FpsLimiter fpsLimiter;
  const int MAX_PHYSICS_STEPS = 6;
  const float DESIRED_FPS = 60.0f;
  const float MS_PER_SECOND = 1000;
  const float DESIRED_FRAMETIME = MS_PER_SECOND/DESIRED_FPS;
  const float MAX_DELTA_TIME = 1.0f;
  fpsLimiter.setMaxFPS(DESIRED_FPS);
  float previousTicks = SDL_GetTicks();

  //Rectangle r(glm::vec2(2.5,10),glm::vec2(5,5));
  //Circle c(glm::vec2(glm::vec2(10,10)),5);
  //std::cout << r.intersects(c) << " " << c.intersects(r) << std::endl;
  //std::cout << c.intersects(r) << std::endl;

  while(m_gameState != GameState::EXIT) {
    //Get new input, and time
    fpsLimiter.begin();
    float newTicks = SDL_GetTicks();
    float frameTime = newTicks - previousTicks;
    previousTicks = newTicks;
    float totalDeltaTime = frameTime / DESIRED_FRAMETIME;
    int i = 0;
    //Update
    while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
      m_inputManager.update();
      processInput();
      float deltaTime = std::min(totalDeltaTime,MAX_DELTA_TIME);
      //TODO: Update objects
      switch(m_gameState) {
      case GameState::MAIN_MENU:
        updateButtons(m_menuButtons);
        break;
      case GameState::LEVEL_SELECT:
        updateButtons(m_levelButtons);
        break;
      case GameState::PLAY:
        updateLevel(deltaTime);
      default:
        break;
      }
      totalDeltaTime -= deltaTime;
      i++;
    }
    //Draw
    //m_camera.setPosition(glm::vec2(512.0f,384.0f));
    m_camera.update();
    drawGame();
    m_fps = fpsLimiter.end();
  }
}

void SmasherGame::processInput() {
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

void SmasherGame::updateButtons(std::vector<BaseButton>& buttons) {
  glm::vec2 mousePos = m_inputManager.getMouseCoords();
  mousePos = m_camera.convertScreenToWorld(mousePos);
  for (size_t i = 0; i < buttons.size(); i++) {
    if (!buttons[i].hoverFunc(mousePos)) continue;
    if (m_inputManager.isKeyReleased(SDL_BUTTON_LEFT)) {
      buttons[i].clickFunc(*this);
    }
    break;
  }
}

void SmasherGame::updateLevel(float deltaTime) {
  m_level.update();
}
