//
// Created by Victus on 22/3/2025.
//

#ifndef GAME_H
#define GAME_H
#include "Configuration.h"
#include "PlayerCamera.h"
#include "Renderer.h"
#include "Scene.h"
#include "input/InputManager.h"
#include "physics/Physics.h"


class Game {
    Scene* scene;
    Physics* physics;
    Configuration* config{};
    PlayerCamera* playerCamera;
    InputManager inputManager;
    Renderer* renderer;
public:
    Game();
    ~Game();
    void Start();
    void Loop();
    void UpdatePlayerCamera() const;

    void Log() const;
};



#endif //GAME_H
