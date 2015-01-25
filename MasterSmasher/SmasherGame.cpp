#include "SmasherGame.h"

#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>
#include <Bengine/ResourceManager.h>
#include <Bengine/Vertex.h>

#include "Rectangle.h"
#include "Circle.h"

#include "BaseObject.h"

#include <string>
#include <iostream>

SmasherGame::SmasherGame() {}

SmasherGame::~SmasherGame() {}

void SmasherGame::run() {
  initSystems();
  Bengine::Music music = m_audioEngine.loadMusic("Music/gameplaySong.ogg");
  music.play(-1);
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

  if (m_gameState == GameState::MAIN_MENU) drawMenu();

  m_textureProgram.unuse();
  // Swap our buffer and draw everything to the screen!
  m_window.swapBuffer();
}

void SmasherGame::drawMenu() {
  //Draw background
  const glm::vec4 uvRect(0.0f,0.0f,1.0f,1.0f);
  glm::vec4 destRect(0.0f, 0.0f, m_screenWidth, m_screenHeight);
  //Write text
  char buffer[256];
  sprintf(buffer, "NUM");
  m_spriteBatch.begin(Bengine::GlyphSortType::NONE);
  m_hudSpriteBatch.begin();
  /*m_spriteFont->draw(m_hudSpriteBatch, buffer,
                     glm::vec2(250,250), glm::vec2(1.0f), 0.0f,
                     Bengine::ColorRGBA8(255,255,255,255));*/
  m_spriteBatch.draw(destRect,uvRect,
                      Bengine::ResourceManager::getTexture("Backgrounds/bgMenu.png").id,
                     0.0f,Bengine::WHITE_COLOR);
  //Menu objects
  for (size_t i = 0; i < m_menuObjects.size(); i++) {
    m_menuObjects[i].draw(m_spriteBatch);
  }
  m_spriteBatch.end();
  m_hudSpriteBatch.end();
  m_spriteBatch.renderBatch();
  m_hudSpriteBatch.renderBatch();
}

void SmasherGame::initMainMenu() {
  m_menuObjects.reserve(NUM_MENU_OBJECTS); //Avoid copying around when allocating @ push
  m_menuObjects.emplace_back();
  m_menuObjects[0].initialize(
    Bengine::ResourceManager::getTexture("Menu/LevelSelect.png"),
    ShapeType::RECTANGLE, glm::vec2(500,500), 1, 1, 1, Bengine::ColorRGBA8(250,125,125,255));
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
  m_spriteFont = new Bengine::SpriteFont("Fonts/kenpixel_mini.ttf",64);
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
    m_inputManager.update();
    processInput();
    int i = 0;
    //Update
    while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
      float deltaTime = std::min(totalDeltaTime,MAX_DELTA_TIME);
      //TODO: Update objects
      switch(m_gameState) {
      case GameState::MAIN_MENU:
        updateMenu(deltaTime);
        break;
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

void SmasherGame::updateMenu(float deltaTime) {

}
