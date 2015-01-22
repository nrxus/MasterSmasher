#pragma once

#include <Bengine/AudioEngine.h>
#include <Bengine/Camera2D.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/InputManager.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/SpriteFont.h>
#include <Bengine/Window.h>

enum class GameState { MAIN_MENU, TUTORIAL, LEVEL_SELECT, PLAY, SCORE_SCREEN, EXIT };

class SmasherGame
{
public:
  SmasherGame();
  ~SmasherGame();

  void run();

private:
  void drawGame();
  void drawMenu();
  void initShaders();
  void initSystems();
  void gameLoop();
  void processInput();
  void updateMenu(float deltaTime);

  int m_screenWidth = 1024;
  int m_screenHeight = 768;
  float m_fps = 0.0f;

  Bengine::AudioEngine m_audioEngine;
  Bengine::Camera2D m_camera;
  Bengine::Camera2D m_hudCamera;
  Bengine::InputManager m_inputManager;
  Bengine::GLSLProgram m_textureProgram;
  //Bengine::SpriteBatch m_bgSpriteBatch; //Background
  Bengine::SpriteBatch m_spriteBatch;
  Bengine::SpriteBatch m_hudSpriteBatch; //Fonts are sorted different
  Bengine::SpriteFont* m_spriteFont;
  Bengine::Window m_window;

  GameState m_gameState = GameState::MAIN_MENU;
};
