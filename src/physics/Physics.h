//
// Created by Victus on 24/3/2025.
//

#ifndef PHYSICS_H
#define PHYSICS_H

#include "Gravity.h"
#include "../entities/Player.h"
#include "../entities/Planet.h"

class Physics {
    Player* player;
    Planet* planet;
    Vector3 CalculatePlanetGravity() const;
    void AlignPlayerToPlanet() const;

    void CheckCollision() const;

public:
    Physics(Player* player, Planet* planet);
    ~Physics();
    void Apply() const;
};



#endif //PHYSICS_H
