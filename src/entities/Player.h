//
// Created by Victus on 22/3/2025.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <string>

#include "raylib.h"
#include "raymath.h"
#include "../physics/RigidBody.h"

struct MovementResult {
    Vector3 localInput;
    Vector3 worldMovement;
};

class Player : public RigidBody {
    bool isJumping = false;
    bool isMoving;
    float yaw;
    float rotationAngle = 0.0f; // Current facing angle
    float targetRotationAngle = 0.0f; // Target angle to rotate toward
    float rotationSpeed = 1.0f;
    float movementSpeed = 30.0f;
    Vector3 lastMoveDirection;
    Vector3 groundNormal = {0, 1, 0}; // Default to world "up"
    Vector3 movementDirection = Vector3Zero();
    Vector3 targetFacingDirection = {0, 0, 1}; // Default forward
public:
    Player(float mass, float radius, Vector3 position);

    Quaternion QuaternionLookRotation(Vector3 forward, Vector3 up);

    ~Player() = default;
    void Jump();

    Vector3 GetLocalInputDirection();

    Vector3 ConvertLocalToWorld(const Vector3 &input);

    Quaternion QuaternionLookAt(Vector3 eye, Vector3 target, Vector3 up);

    void Move();
    void HandleMovementInput(const Vector3 &cameraForward, const Vector3 &cameraRight, float deltaTime);

    void BasicMovement();

    MovementResult UpdateMovement();
    void UpdateRotation(MovementResult result);

    Vector3 GetPlanetaryUpVector();

    void ApplyGravityAlignment();

    float GetRotationAngle() const { return rotationAngle; }
    std::string ToString() const;
    void SetModel(const std::string& path);
    Model GetModel() const;
    void SetYaw(float newYaw);
    void SetGroundNormal(const Vector3& normal);
    Vector3 GetGroundNormal() const;
    void SetJumping(bool jumping) { isJumping = jumping; }
    bool IsJumping() const { return isJumping; }
};



#endif //PLAYER_H
