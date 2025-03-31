//
// Created by Victus on 22/3/2025.
//

#include "Sky.h"

Sky::Sky(): texture(), model() {
}

Sky::~Sky() = default;

Texture2D Sky::GetTexture() const {
    return texture;
}

void Sky::SetTexture(const std::string path) {
    texture = LoadTexture(path.c_str());
}

void Sky::SetMesh(const Mesh &mesh) {
    model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

Model Sky::GetModel() const {
    return model;
}
