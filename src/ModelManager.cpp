//
// Created by Victus on 23/3/2025.
//

#include "ModelManager.h"


void ModelManager::LoadSky(std::string path, Sky *sky) {
    Mesh skyMesh = GenMeshSphere(10.0f, 32, 32); // Large sphere
    sky->SetTexture(path);
    sky->SetMesh(skyMesh);
}
