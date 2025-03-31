//
// Created by Victus on 24/3/2025.
//

#ifndef PLAYERCAMERA_H
#define PLAYERCAMERA_H
#include "raylib.h"
#include "entities/Player.h"
#include "input/Mouse.h"


class PlayerCamera : public Camera {
    Player* player;
    Vector3 offset = { 30.0f, 30.0f, 30.0f };
    float yaw = 0;
    float pitch = 0;
    // Configurable parameters with sensible defaults
    float mouseSensitivity = 0.003f;
    float minPitch = -PI/4;
    float maxPitch = PI/4;
    float distance = 8.0f;  // Distance from playerfloat zoomSpeed = 0.5f;
    float zoomSpeed = 0.5f;
    float minDistance = 2.0f;
    float maxDistance = 100.0f;
    void HandleMouseInput();
    void UpdatePosition();
    void UpdateCenter();
public:
    explicit PlayerCamera(Player* player);

    void HandleMouseWheel();

    void Update();
    Vector3 GetCameraForward() const;
    Vector3 GetCameraRight() const;
};



#endif //PLAYERCAMERA_H
