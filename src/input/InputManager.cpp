//
// Created by Victus on 24/3/2025.
//

#include "InputManager.h"

InputManager::InputManager(Player* player, PlayerCamera *playerCamera) : player(player),
playerCamera(playerCamera) { }

InputManager::~InputManager() = default;

void InputManager::MovePlayer() const {
    if (IsKeyDown(KEY_SPACE)) { player->Jump(); }
    playerCamera->Update();
    player->Move();
}
