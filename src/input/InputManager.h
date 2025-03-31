//
// Created by Victus on 24/3/2025.
//

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include "Keyboard.h"
#include "Mouse.h"
#include "../entities/Player.h"
#include "../PlayerCamera.h"

class InputManager {
    Mouse mouse;
    Keyboard keyboard;
    Player* player;
    PlayerCamera* playerCamera;
public:
    InputManager(Player* player, PlayerCamera* playerCamera);
    ~InputManager();
    Mouse& GetMouse() { return mouse; }
    void MovePlayer() const;
};



#endif //INPUTMANAGER_H
