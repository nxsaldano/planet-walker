//
// Created by Victus on 22/3/2025.
//

#ifndef SKY_H
#define SKY_H
#include <string>

#include "raylib.h"


class Sky {
    Texture2D texture;
    Model model;
public:
    Sky();
    ~Sky();
    Texture2D GetTexture() const;
    void SetTexture(std::string path);
    void SetMesh(const Mesh &mesh);
    void SetModel(const std::string& path);
    Model GetModel() const;
};



#endif //SKY_H
