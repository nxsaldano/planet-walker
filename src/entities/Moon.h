//
// Created by Victus on 22/3/2025.
//

#ifndef MOON_H
#define MOON_H
#include <string>

#include "raylib.h"


class Moon {
    Model model;
    // float rotationSpeed = 0.02f;
    // float orbitRadius = 28.0f;
    // float rotation = 0.0f;          // Rotation of moon around itself
    // float orbitRotation = 0.0f;
public:
    Moon();
    ~Moon();
    void SetModel(const std::string& path);
};



#endif //MOON_H
