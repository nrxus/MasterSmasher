#pragma once

#include <Bengine/AudioEngine.h>
#include <Bengine/Camera2D.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/InputManager.h>
#include <Bengine/ParticleBatch2D.h>
#include <Bengine/ParticleEngine2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/SpriteFont.h>
#include <Bengine/Window.h>

#include "Bullet.h"
#include "Level.h"
#include "Player.h"

class Zombie;

enum class GameState { PLAY,EXIT };

class ZombieGame
{
public:
  ZombieGame();
  ~ZombieGame();

/// Runs the game
  void run();

private:
/// Initializes the core systems
  void initSystems();

// Initalizes the level and sets up everything
  void initLevel();

/// Initializes the shaders
  void initShaders();

/// Main game loop for the program
  void gameLoop();

  void updateAgents(float deltaTime);

  void updateBullets(float deltaTime);

  void checkVictory();

/// Handles input processing
  void processInput();

/// Renders the game
  void drawGame();

  void drawHUD();

  void addBlood(const glm::vec2& position, int numParticles);

  //Initialize member variable
  int m_numHumansKilled = 0;
  int m_numZombiesKilled = 0;
  int m_screenWidth = 1024;
  int m_screenHeight = 768;
  float m_fps = 0.0f;

  GameState m_gameState = GameState::PLAY;
  Player* m_player = nullptr;

  //Declare other member variables
  int m_currentLevel;

  Bengine::AudioEngine m_audioEngine;
  Bengine::Camera2D m_camera; ///< Main Camera
  Bengine::Camera2D m_hudCamera;
  Bengine::InputManager m_inputManager; ///< Handles input
  Bengine::GLSLProgram m_textureProgram; ///< The shader program
  Bengine::ParticleEngine2D m_particleEngine;
  Bengine::ParticleBatch2D* m_bloodParticleBatch;
  Bengine::SpriteBatch m_agentSpriteBatch;
  Bengine::SpriteBatch m_hudSpriteBatch;
  Bengine::SpriteFont* m_spriteFont;
  Bengine::Window m_window; ///< The game window

  std::vector<Human*> m_humans;
  std::vector<Zombie*> m_zombies;
  std::vector<Level*> m_levels;
  std::vector<Bullet> m_bullets;
};
