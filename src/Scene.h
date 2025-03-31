//
// Created by Victus on 24/3/2025.
//

#ifndef SCENE_H
#define SCENE_H
#include "entities/Planet.h"
#include "entities/Player.h"
#include "entities/Sky.h"


class Scene {
    Player* player;
    Planet* planet;
    Sky* sky;
    // Moon* moon;
public:
    Scene();
    ~Scene();
    void Load() const;
    Player* GetPlayer() const;
    Planet* GetPlanet() const;
    Sky* GetSky() const;
};



#endif //SCENE_H
