//
// Created by Victus on 24/3/2025.
//

#include "PlayerCamera.h"

#include "raymath.h"

PlayerCamera::PlayerCamera(Player *player) : Camera(), player(player) {
    // Initialize camera properties
    up = {0.0f, 1.0f, 0.0f};
    fovy = 60.0f;
    projection = CAMERA_PERSPECTIVE;
    yaw = -90.0f;    // Set initial yaw so the camera is behind the player
    pitch = 10.0f;   // Adjust pitch as needed
}

void PlayerCamera::HandleMouseWheel() {
    float wheel = GetMouseWheelMove();
    if(wheel != 0) {
        // Adjust distance with zoom speed and clamp it
        distance = Clamp(distance - (wheel * zoomSpeed), minDistance, maxDistance);
    }
}

void PlayerCamera::Update() {
    // HandleMouseInput();
    HandleMouseWheel();
    UpdatePosition();
}

void PlayerCamera::HandleMouseInput() {
    if (!IsKeyDown(KEY_LEFT_CONTROL)) {  // Hold right mouse to rotate
        const Vector2 mouseDelta = GetMouseDelta();
        yaw += mouseDelta.x * 0.1f;      // Horizontal rotation
        // Update player's yaw to match camera rotation
        player->SetYaw(yaw);
        pitch = Clamp(pitch - mouseDelta.y * 0.1f, -30.0f, 80.0f);  // Vertical limits
    }
}

void PlayerCamera::UpdatePosition() {
    // Calculate camera position based on spherical coordinates
    // Vector3 targetPos = Vector3Scale(player->GetPosition(), 10.f);
    Vector3 targetPos = player->GetPosition();


    // Convert yaw/pitch to radians
    float yawRad = DEG2RAD * yaw;
    float pitchRad = DEG2RAD * pitch;

    // Calculate orbital position
    position = {
        targetPos.x + distance * cosf(yawRad) * cosf(pitchRad),
        targetPos.y + distance * sinf(pitchRad),
        targetPos.z + distance * sinf(yawRad) * cosf(pitchRad)
    };

    // Always look at the player's offset position
    target = targetPos;
}


Vector3 PlayerCamera::GetCameraForward() const {
    return Vector3Normalize((Vector3){std::sin(yaw), 0, std::cos(yaw)});
}

Vector3 PlayerCamera::GetCameraRight() const {
    return Vector3Normalize((Vector3){std::cos(yaw), 0, -std::sin(yaw)});
}

