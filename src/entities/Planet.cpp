//
// Created by Victus on 22/3/2025.
//

#include "Planet.h"

Planet::Planet(const float radius): model(), position(), center(), radius(radius) { }

Planet::~Planet() = default;

void Planet::SetModel(const std::string& path) {
    model = LoadModel(path.c_str());
}

Model Planet::GetModel() const {
    return model;
}

Vector3 Planet::GetPosition() const {
    return position;
}

Vector3 Planet::GetCenter() const {
    return center;
}

float Planet::GetRadius() const {
    return radius;
}
