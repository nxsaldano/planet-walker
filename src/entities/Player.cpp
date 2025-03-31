//
// Created by Victus on 22/3/2025.
//

#include "Player.h"

#include <sstream>

#include "raymath.h"

Player::Player(const float mass, const float radius, const Vector3 position) : isMoving(false), yaw(180.0f),
lastMoveDirection(){
    SetMass(mass);
    this->radius = radius;
    SetPosition(position);
    linearDamping = 0.6f;
    rotation = Vector3Zero();
    velocity = Vector3Zero();
    isGrounded = false;
    isAwake = false;
    acceleration = Vector3Zero();
    forceAccum = Vector3Zero();
}

Quaternion Player::QuaternionLookRotation(Vector3 forward, Vector3 up) {
    Vector3 right = Vector3CrossProduct(up, forward);
    right = Vector3Normalize(right);
    Vector3 correctedUp = Vector3CrossProduct(forward, right);

    Matrix mat = {
        right.x, right.y, right.z, 0,
        correctedUp.x, correctedUp.y, correctedUp.z, 0,
        forward.x, forward.y, forward.z, 0,
        0, 0, 0, 1
    };

    return QuaternionFromMatrix(mat);
}

/*void Player::Jump() {
    if (isGrounded) {
        if (IsKeyPressed(KEY_SPACE)) {
            // Use world up instead of ground normal
            // Get ground normal from collision system
            // Vector3 jumpDirection = groundNormal;
            const Vector3 jumpDirection = GetGroundNormal();
            const float jumpForce = 1000.0f;

            // Clear existing vertical velocity
            Vector3 velocity = GetVelocity();
            velocity.y = 0;
            SetVelocity(velocity);

            // Apply upward impulse
            AddForce(Vector3Scale(jumpDirection, jumpForce * mass));
            isGrounded = false;
        }
    }
}*/

void Player::Jump() {
    if (isGrounded) {
        if (IsKeyPressed(KEY_SPACE)) {
            SetJumping(true);
            lastMoveDirection = Vector3Normalize(lastMoveDirection);
            Vector3 velocity = GetVelocity();
            Vector3 groundNormal = GetGroundNormal();

            // Remove velocity along the ground normal
            velocity = Vector3Subtract(velocity, Vector3Scale(groundNormal, Vector3DotProduct(velocity, groundNormal)));
            SetVelocity(velocity);

            // Apply jump impulse
            Vector3 jumpImpulse = Vector3Scale(groundNormal, 30.0f); // Increased strength
            SetVelocity(Vector3Add(velocity, jumpImpulse));

            isGrounded = false;
        }
    }
}

void Player::Move() {
    // 1. Get movement input and apply movement
    MovementResult result = UpdateMovement(); // Store as MovementResult
    Jump();

    // 2. Update rotation using both values from the result
    UpdateRotation(result);
}

void Player::UpdateRotation(MovementResult result) {
    Vector3 normalizedGravity = Vector3Normalize(gravityDirection);
    Vector3 upVector = Vector3Negate(normalizedGravity);
    Quaternion gravityAlign = QuaternionFromVector3ToVector3({0, 1, 0}, upVector);

    if (isGrounded) {
        if (Vector3Length(result.localInput) > 0.01f) {
            // Get input direction (already normalized)
            Vector2 inputDir = { result.localInput.x, result.localInput.z };

            // Calculate yaw angle from input
            float yawAngle = atan2(inputDir.x, inputDir.y); // Negate X for correct left/right

            // Create pure yaw rotation around planetary up
            Quaternion yawRot = QuaternionFromAxisAngle(upVector, yawAngle);

            // Combine with gravity alignment
            orientation = QuaternionMultiply(yawRot, gravityAlign);
        }
    } else {
        // Airborne rotation (existing code)
        if (Vector3Length(result.worldMovement) > 0.01f) {
            Vector3 forward = Vector3Normalize(result.worldMovement);
            Vector3 right = Vector3Normalize(Vector3CrossProduct(upVector, forward));
            Vector3 up = Vector3Normalize(Vector3CrossProduct(forward, right));

            Matrix rotMat = {
                right.x,    up.x,       forward.x, 0,
                right.y,    up.y,       -forward.y, 0,
                right.z,    up.z,       forward.z, 0,
                0,          0,          0,         1
            };

            Quaternion targetRot = QuaternionFromMatrix(rotMat);
            orientation = QuaternionSlerp(orientation, targetRot, 0.1f);
        }
    }
}

MovementResult Player::UpdateMovement() {
    int zMovement = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
    int xMovement = IsKeyDown(KEY_A) - IsKeyDown(KEY_D);
    Vector3 inputDirection = { static_cast<float>(xMovement), 0.0f, static_cast<float>(zMovement) };

    if (Vector3Length(inputDirection) > 0.01f) {
        inputDirection = Vector3Normalize(inputDirection);
    }

    // Compute gravity-aligned orientation
    Vector3 normalizedGravity = Vector3Normalize(gravityDirection);
    Vector3 upVector = Vector3Negate(normalizedGravity);
    Quaternion gravityAlign = QuaternionFromVector3ToVector3({0, 1, 0}, upVector);

    // Extract vectors from gravity alignment
    Matrix rotMat = QuaternionToMatrix(gravityAlign);
    Vector3 forward = {rotMat.m8, rotMat.m9, rotMat.m10};
    Vector3 right = { rotMat.m0, -rotMat.m1, rotMat.m2 }; // FIXED HERE

    // Calculate world movement
    Vector3 worldMovement = Vector3Add(
        Vector3Scale(forward, inputDirection.z),
        Vector3Scale(right, inputDirection.x)
    );

    if (Vector3Length(worldMovement) > 0.01f) {
        worldMovement = Vector3Normalize(worldMovement);
        position = Vector3Add(position, Vector3Scale(worldMovement, movementSpeed * GetFrameTime()));
    }

    // Return both values in a struct
    return {inputDirection, worldMovement};
}

void Player::SetModel(const std::string& path) {
    model = LoadModel(path.c_str());
    center = Vector3Add(position, CalculateMeshCenter(model));
}

Model Player::GetModel() const {
    return model;
}

void Player::SetYaw(const float newYaw) {
    yaw = newYaw;
}

void Player::SetGroundNormal(const Vector3 &normal) {
    groundNormal = Vector3Normalize(normal);
}

Vector3 Player::GetGroundNormal() const {
    return groundNormal;
}

std::string Player::ToString() const {
    std::ostringstream oss;
    oss << "Player:\n";
    oss << "  Rotation Angle: " << rotationAngle << "\n";
    oss << "  Mass: (" << mass << ")\n";
    oss << "  Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
    oss << "  Center: (" << center.x << ", " << center.y << ", " << center.z << ")\n";
    oss << "  Velocity: (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")\n";
    oss << "  Acceleration: (" << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << ")\n";
    oss << "  Radius: " << radius << "\n";
    oss << "  Is Moving: " << (isMoving ? "true" : "false") << "\n";
    oss << "  Is Grounded: " << (isGrounded ? "true" : "false") << "\n";
    oss << "  Orientation: (" << orientation.x << ", " << orientation.y << ", " << orientation.z << ", "
    << orientation.w << ")\n";
    oss << "  Force Accumulation: (" << forceAccum.x << ", " << forceAccum.y << ", " << forceAccum.z << ")\n";
    return oss.str();
}

void Player::BasicMovement() {
    // Get movement input from keys (returns 1, 0, or -1)
    int zMovement = IsKeyDown(KEY_W) - IsKeyDown(KEY_S);
    int xMovement = IsKeyDown(KEY_A) - IsKeyDown(KEY_D);

    // Create a movement vector from the input
    Vector3 movement = { static_cast<float>(xMovement), 0.0f, static_cast<float>(zMovement) };

    // If any movement, normalize the vector (to avoid faster diagonal movement)
    if (movement.x != 0 || movement.z != 0) {
        movement = Vector3Normalize(movement);
        // Scale by movementSpeed and add to the player's position
        position = Vector3Add(position, Vector3Scale(movement, movementSpeed));
    }

    // Compute the new angle (in radians) using atan2.
    // Here we assume:
    // - 0 degrees (0 rad) means facing forward (along +Z).
    // - A positive X (to the right) rotates the angle clockwise.
    float angleRad = atan2(movement.x, movement.z); // Note: x first, z second
    float angleDeg = RAD2DEG * angleRad;            // Convert to degrees

    // Update the player's rotation angle.
    rotationAngle = angleDeg;
}

