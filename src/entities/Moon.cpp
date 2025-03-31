//
// Created by Victus on 22/3/2025.
//

#include "Moon.h"

void Moon::SetModel(const std::string& path) {
    model = LoadModel(path.c_str());
}
