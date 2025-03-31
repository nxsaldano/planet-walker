//
// Created by Victus on 22/3/2025.
//

#ifndef PLANET_H
#define PLANET_H
#include <string>

#include "raylib.h"
#include "raymath.h"


class Planet {
    Model model;
    Vector3 position = Vector3Zero();
    Vector3 center = position;
    float radius;
public:
    explicit Planet(float radius);
    ~Planet();
    void SetModel(const std::string& path);
    Model GetModel() const;
    Vector3 GetPosition() const;
    Vector3 GetCenter() const;
    float GetRadius() const;
};



#endif //PLANET_H
