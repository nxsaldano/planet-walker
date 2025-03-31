//
// Created by Victus on 22/3/2025.
//

#include "Configuration.h"

Configuration::Configuration(int screenWidth, int screenHeight): screenWidth(screenWidth),
screenHeight(screenHeight) { }

Configuration::~Configuration() = default;

std::array<int, 2> Configuration::GetResolution() const {
    return { screenWidth, screenHeight };
}
