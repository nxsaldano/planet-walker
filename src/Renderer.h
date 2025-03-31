//
// Created by Victus on 22/3/2025.
//

#ifndef RENDERER_H
#define RENDERER_H
#include "PlayerCamera.h"
#include "Scene.h"


class Renderer {
    PlayerCamera* playerCamera;
    Scene* scene;
    bool f1WasPressed = false;
    bool lastF1State = false;
    bool showDebug = false;
    float alpha = 1.0f;        // Start fully black (1.0 = fully opaque)
    float fadeSpeed = 0.7f;    // Fade out speed (units per second)
    void DrawSky() const;
    void DrawPlanet() const;
    void DrawPlayerWithRotation() const;
    void DrawDebugSpheres() const;

    void DrawDebugVectors() const;

    void ToggleDebug();

    static Vector3 MatrixTransform(Vector3 v, Matrix m);

    static BoundingBox ScaleBoundingBox(const BoundingBox &box, float scale);

    void FadeIn();

    // void DrawMoon() const;
    // void ShowGrid() const;
public:
    Renderer(Scene* scene, PlayerCamera* playerCamera);
    ~Renderer();
    void DrawScene();
};



#endif //RENDERER_H
