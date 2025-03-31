//
// Created by Victus on 22/3/2025.
//

#include "Renderer.h"

#include <iostream>

#include "raymath.h"
#include "rlgl.h"


Renderer::Renderer(Scene* scene, PlayerCamera* playerCamera) : playerCamera(playerCamera), scene(scene) {}

Renderer::~Renderer() = default;

void Renderer::DrawScene() {
    BeginDrawing();
        ClearBackground(RAYWHITE);
        BeginMode3D(static_cast<Camera3D>(*playerCamera));
            ToggleDebug();
            DrawSky();
            // DrawDebugSpheres();
            DrawPlanet();
            DrawPlayerWithRotation();
        EndMode3D();
    FadeIn();
    //FadeIn();
    EndDrawing();
}

void Renderer::DrawSky() const {
    rlDisableBackfaceCulling();
        DrawModel(scene->GetSky()->GetModel(), Vector3Zero(), 80.0f, WHITE);
    rlEnableBackfaceCulling();
}

void Renderer::DrawPlanet() const {

    // Define the desired transparency level (0.0f = fully transparent, 1.0f = fully opaque)
    // float alpha = 0.5f; // 50% transparency

    // Apply the Fade function to the WHITE color to set the desired transparency
    // Color transparentWhite = Fade(WHITE, alpha);

    DrawModel(scene->GetPlanet()->GetModel(),
        scene->GetPlanet()->GetPosition(),
        0.5f, WHITE);
}

void Renderer::DrawPlayerWithRotation() const {
    // Vector3 axis = {0, 1, 0}; // Y-axis rotation
    //
    // DrawModelEx(
    //     scene->GetPlayer()->GetModel(),
    //     scene->GetPlayer()->GetPosition(),
    //     axis,
    //     scene->GetPlayer()->GetRotationAngle(),
    //     {1.0f, 1.0f, 1.0f},
    //     WHITE
    // );

    // Convert quaternion to axis-angle for Raylib
    Vector3 rotationAxis;
    float quaternionRotationAngle;
    QuaternionToAxisAngle(scene->GetPlayer()->GetOrientation(), &rotationAxis, &quaternionRotationAngle);

    DrawModelEx(
        scene->GetPlayer()->GetModel(),
        scene->GetPlayer()->GetPosition(),
        rotationAxis,
        quaternionRotationAngle * RAD2DEG, // Convert to degrees
        {1.0f, 1.0f, 1.0f},
        WHITE
    );
}

void Renderer::DrawDebugSpheres() const {
    // Red sphere at the Planet origin (center)
    // DrawSphere(scene->GetPlanet()->GetPosition(), 1.0f, RED);
    // BoundingBox box = GetModelBoundingBox(scene->GetPlanet()->GetModel());
    // BoundingBox scaledBox = ScaleBoundingBox(box, 0.5f);  // Adjust 0.5f to your desired scale factor
    // DrawBoundingBox(scaledBox, RED);
    // Draw bounding spheres (for debugging)
    DrawSphereWires(scene->GetPlanet()->GetCenter(), scene->GetPlanet()->GetRadius(),
        16, 16, GREEN); // Earth's bounding sphere
    DrawSphere(scene->GetPlayer()->GetPosition(), 0.2f, RED); // Red sphere at the origin (feet)
    // DrawSphereWires(scene->GetPlayer()->GetCenter(), scene->GetPlayer()->GetRadius(),
    //    16, 16, RED); // Player's bounding sphere
}

void Renderer::DrawDebugVectors() const {
    Player* player = scene->GetPlayer();
    Vector3 playerPos = player->GetPosition();

    // 1. Local XYZ Axes (at player's position)
    Matrix rotation = QuaternionToMatrix(player->GetOrientation());
    Matrix transform = MatrixMultiply(rotation, MatrixTranslate(playerPos.x, playerPos.y, playerPos.z));

    DrawLine3D(MatrixTransform({0,0,0}, transform), MatrixTransform({3,0,0},
        transform), RED);   // X
    DrawLine3D(MatrixTransform({0,0,0}, transform), MatrixTransform({0,3,0},
        transform), ORANGE); // Y
    DrawLine3D(MatrixTransform({0,0,0}, transform), MatrixTransform({0,0,3},
        transform), BLUE);  // Z

    // 2. Ground Normal (Yellow)
    Vector3 planetCenter = scene->GetPlanet()->GetCenter();
    DrawLine3D(playerPos, planetCenter, YELLOW);

    // 3. Velocity (Red, scaled)
    Vector3 velocity = Vector3Scale(player->GetVelocity(), 0.1f);
    DrawLine3D(playerPos, Vector3Add(playerPos, velocity), VIOLET);
}

void Renderer::ToggleDebug() {
    bool currentF1State = IsKeyPressed(KEY_F1);

    // Toggle only on key press (not hold)
    if (currentF1State && !lastF1State) {
        showDebug = !showDebug; // Flip state once
    }

    lastF1State = currentF1State; // Remember for next frame

    if (showDebug) {
        DrawDebugVectors();
        DrawDebugSpheres();
        DrawText(scene->GetPlayer()->ToString().c_str(), 20, 20, 20, WHITE);
    }
}

// Add these helper functions if missing
Vector3 Renderer::MatrixTransform(Vector3 v, Matrix m) {
    return {
        m.m0*v.x + m.m4*v.y + m.m8*v.z + m.m12,
        m.m1*v.x + m.m5*v.y + m.m9*v.z + m.m13,
        m.m2*v.x + m.m6*v.y + m.m10*v.z + m.m14
    };
}

// Helper function to scale a bounding box
BoundingBox Renderer::ScaleBoundingBox(const BoundingBox &box, float scale)
{
    // Calculate the center of the bounding box
    Vector3 center = {
        (box.min.x + box.max.x) / 2.0f,
        (box.min.y + box.max.y) / 2.0f,
        (box.min.z + box.max.z) / 2.0f
    };
    // Calculate the half extents (size/2)
    Vector3 halfSize = {
        (box.max.x - box.min.x) / 2.0f,
        (box.max.y - box.min.y) / 2.0f,
        (box.max.z - box.min.z) / 2.0f
    };
    // Scale the half extents
    halfSize = Vector3Scale(halfSize, scale);
    // Create the new, scaled bounding box
    BoundingBox scaledBox;
    scaledBox.min = Vector3Subtract(center, halfSize);
    scaledBox.max = Vector3Add(center, halfSize);
    return scaledBox;
}

void Renderer::FadeIn() {

    float deltaTime = GetFrameTime();

    if (alpha > 0.0f) {
        alpha -= fadeSpeed * deltaTime;
        if (alpha < 0.0f) alpha = 0.0f;
    }

    DrawRectangle(0, 0, 1920, 1080, Fade(BLACK, alpha));

}
