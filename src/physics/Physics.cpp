//
// Created by Victus on 24/3/2025.
//

#include "Physics.h"

#include <iostream>

Physics::Physics(Player* player, Planet* planet): player(player), planet(planet) {}

Physics::~Physics() = default;

void Physics::Apply() const {
    const float deltaTime = GetFrameTime();
    Gravity planetGravity = Gravity(CalculatePlanetGravity());
    player->SetGravityDirection(Vector3Subtract(planet->GetCenter(), player->GetPosition()));
    //Gravity planetGravity = Gravity({0,-0.1f,0});
    planetGravity.UpdateForce(player, deltaTime);
    player->Integrate(deltaTime);
    // AlignPlayerToPlanet();
    CheckCollision();
}

Vector3 Physics::CalculatePlanetGravity() const {
    Vector3 gravityDir = Vector3Subtract(player->GetPosition(), planet->GetPosition());
    gravityDir = Vector3Normalize(gravityDir);

    // Adjust gravity strength (e.g., 9.8f for Earth-like gravity)
    return Vector3Scale(gravityDir, -39.8f);
}

void Physics::AlignPlayerToPlanet() const {
    Vector3 groundNormal = player->GetGroundNormal();
    Vector3 forward = {0,0,1};

    // Create rotation aligning up to normal while preserving forward
    Quaternion rot = QuaternionFromVector3ToVector3(
        {0, 1, 0},  // World up
        groundNormal // Planet surface up
    );

    // Preserve forward direction relative to surface
    forward = Vector3RotateByQuaternion(forward, rot);
    Quaternion forwardRot = QuaternionFromVector3ToVector3(
        {0, 0, 1},  // Default forward
        forward
    );

    player->SetOrientation(QuaternionMultiply(rot, forwardRot));
}

void Physics::CheckCollision() const {
    const Vector3 planetCenter = planet->GetCenter();
    const float planetRadius = planet->GetRadius();
    const Vector3 playerPosition = player->GetPosition();
    const Vector3 playerCenter = player->GetCenter();
    const float playerRadius = player->GetRadius();
    // Check collision using sphere radii
    const bool isColliding = CheckCollisionSpheres(
        planetCenter, planetRadius, // Planet's sphere
        playerCenter, playerRadius // Player's sphere
    );
    if (isColliding) {
        Vector3 normal = Vector3Normalize(Vector3Subtract(playerPosition, planetCenter));

        // Update player's ground normal
        player->SetGroundNormal(normal);

        // 1. Position Correction (with larger offset)
        const float safetyOffset = 0.01f; // Increased from 0.001f
        player->SetPosition(Vector3Add(
            planetCenter,
            Vector3Scale(normal, planetRadius + safetyOffset)
        ));

        // 2. Velocity Resolution with Energy Threshold
        Vector3 velocity = player->GetVelocity();
        float velocityAlongNormal = Vector3DotProduct(velocity, normal);
        TraceLog(LOG_INFO, std::to_string(velocityAlongNormal).c_str());

        // Only resolve if moving TOWARD the planet (avoid upward jitter)
        if (velocityAlongNormal < 0) {
            if (!player->IsJumping()) {  // Add IsJumping() method to track jump state
            // Split velocity into normal/tangential components
            Vector3 normalVel = Vector3Scale(normal, velocityAlongNormal);
            Vector3 tangentVel = Vector3Subtract(velocity, normalVel);

            // Apply restitution and friction
            const float restitution = 0.6f;
            const float friction = 0.3f;
            Vector3 newNormalVel = Vector3Scale(normalVel, -restitution);
            Vector3 newTangentVel = Vector3Scale(tangentVel, friction);
            Vector3 newVelocity = Vector3Add(newNormalVel, newTangentVel);


            player->SetVelocity(newVelocity);

            // 3. Velocity Threshold (kill tiny velocities)
            const float velocityEpsilon = 0.01f;
            if (Vector3Length(newVelocity) < velocityEpsilon) {
                newVelocity = Vector3Zero();
            }

            player->SetVelocity(newVelocity);
            }
        }

        // 4. Ground Damping (additional horizontal slowdown)
        const float groundDamping = 0.1f;
        Vector3 horizontalVel = Vector3Subtract(velocity, Vector3Scale(normal, velocityAlongNormal));
        player->SetVelocity(Vector3Add(
            Vector3Scale(horizontalVel, groundDamping),
            Vector3Scale(normal, velocityAlongNormal)
        ));

        player->SetGrounded(true);
    } else {
        player->SetGrounded(false);
    }
}

/*Vector3 Physics::CalculatePlanetGravity() const {
    // Vector from player to planet center
    Vector3 gravityDirection = Vector3Subtract(planet->GetPosition(), player->GetPosition());
    // Distance between player and planet center
    const float distance = Vector3Length(gravityDirection);

    // Normalize gravity direction
    gravityDirection = Vector3Normalize(gravityDirection);
    // Calculate gravitational force magnitude
    // Adjust these constants to get desired gameplay feel
    constexpr float gravitationalConstant = 20.0f;  // Strength of gravity
    constexpr float planetMass = 10000.0f;  // Arbitrary mass value
    // Inverse square law with a max force to prevent extreme acceleration
    float forceMagnitude = gravitationalConstant * planetMass / (distance * distance);
    // forceMagnitude = Clamp(forceMagnitude, 0.0f, 50.0f);  // Clamp to prevent extreme values
    // Scale direction vector by force magnitude
    const Vector3 planetGravity = Vector3Scale(gravityDirection, forceMagnitude);
    return planetGravity;
}*/

/*void Physics::CheckCollision() const {
    const Vector3 planetCenter = planet->GetCenter();
    const float planetRadius = planet->GetRadius();
    const Vector3 playerPosition = player->GetPosition();
    const float playerRadius = player->GetRadius();
    // Check collision between Earth and player
    const bool isColliding = CheckCollisionSpheres(
        planetCenter, planetRadius, // Planet's sphere
        playerPosition, playerRadius // Player's sphere
    );
    if (isColliding) {
        // std::cout << "Collision detected!\n";
        // Calculate collision normal (from Earth to Player)
        constexpr Vector3 normal = {0,1,0};
        // Calculate penetration depth
        const float distance = Vector3Distance(planetCenter, playerPosition);
        const float penetration = planetRadius + playerRadius - distance;
        // Correct position to move player out of the Earth
        player->SetPosition(Vector3Add(playerPosition, Vector3Scale(normal, penetration)));
        // Adjust velocity: reflect along normal with restitution
        const float restitution = 0.8f; // Adjust for bounciness (0.0 to 1.0)
        const Vector3 velocity = player->GetVelocity();
        // Calculate velocity component along the collision normal
        const float velocityAlongNormal = Vector3DotProduct(velocity, normal);
        // Only resolve if moving towards the Earth (to avoid sticking)
        if (velocityAlongNormal < 0) {
            // Apply impulse to reflect velocity
            Vector3 impulse = Vector3Scale(normal, -(1 + restitution) * velocityAlongNormal);
            player->SetVelocity(Vector3Add(velocity, impulse));
            // std::cout << Vector3Length(impulse) << std::endl;
        }
    }
}*/
