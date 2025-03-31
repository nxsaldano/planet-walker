//
// Created by Victus on 24/3/2025.
//

#include "Scene.h"

#include "ModelManager.h"

Scene::Scene(): player(new Player(10.0f, 1.0f, {0,90,0})),
planet(new Planet(50.0f)),
sky(new Sky) {}

Scene::~Scene() = default;

void Scene::Load() const {
    std::string planetModelPath = "../resources/models/planet2.glb";
    // std::string moonModelPath = "../resources/models/moon.glb";
    std::string playerModelPath = "../resources/models/greenman.glb";
    std::string skyTexturePath = "../resources/textures/sky.jpg";

    ModelManager::LoadModel(planetModelPath, planet);
    // ModelManager::LoadModel(moonModelPath, moon);
    ModelManager::LoadModel(playerModelPath, player);
    ModelManager::LoadSky(skyTexturePath, sky);
}

Player* Scene::GetPlayer() const {
    return player;
}

Planet* Scene::GetPlanet() const {
    return planet;
}

Sky* Scene::GetSky() const {
    return sky;
}


