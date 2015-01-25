#pragma once

#include <vector>

#include <Bengine/Camera2D.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/InputManager.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/Timing.h>
#include <Bengine/Window.h>

#include "Bullet.h"

enum class GameState {PLAY, EXIT};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    void run();

private:
    void drawGame();
    void gameLoop();
    void initSystems();
    void initShaders();
    void processInput();

    int _screenWidth;
    int _screenHeight;
    float _fps;
    float _maxFPS;
    float _time;

    GameState _gameState;

    Bengine::Camera2D _camera;
    Bengine::FpsLimiter _fpsLimiter;
    Bengine::GLSLProgram _colorProgram;
    Bengine::InputManager _inputManager;
    Bengine::SpriteBatch _spriteBatch;
    Bengine::Window _window;

    std::vector<Bullet> _bullets;
};
