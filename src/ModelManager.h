//
// Created by Victus on 23/3/2025.
//

#ifndef MODELMANAGER_H
#define MODELMANAGER_H
#include <string>

#include "entities/Sky.h"


class ModelManager {
public:
    template <typename Entity>
    static void LoadModel(std::string path, Entity* entity) {
        entity->SetModel(path);
    }
    static void LoadSky(std::string path, Sky* sky);
};



#endif //MODELMANAGER_H
