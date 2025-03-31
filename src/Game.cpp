//
// Created by Victus on 22/3/2025.
//

#include "Game.h"

#include "input/InputManager.h"

Game::Game() : scene(new Scene),
physics(new Physics(scene->GetPlayer(), scene->GetPlanet())),
config(new Configuration(1280, 720)),
playerCamera(new PlayerCamera(scene->GetPlayer())),
inputManager(scene->GetPlayer(), playerCamera),
renderer(new Renderer(scene, playerCamera)) {}

Game::~Game() = default;

void Game::Start() {
    const std::array<int, 2> resolution = config->GetResolution();
    InitWindow(resolution[0], resolution[1], "PlanetWalker");
    SetTargetFPS(60);
    scene->Load();
    Loop();
}

void Game::Loop() {
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        Mouse::EnableCursorHiding();
        UpdatePlayerCamera();
        inputManager.MovePlayer();
        physics->Apply();
        // Log();
        // UpdateMoonRotation();
        renderer->DrawScene();
    }
    CloseWindow();
}

void Game::UpdatePlayerCamera() const {
    UpdateCamera(playerCamera, CAMERA_THIRD_PERSON);
    playerCamera->Update();
}

void Game::Log() const {
    Vector3 pos = scene->GetPlayer()->GetPosition();
    TraceLog(LOG_INFO, "Player position: { %.2f, %.2f, %.2f }", pos.x, pos.y, pos.z);
};