//
// Created by Victus on 22/3/2025.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <array>


class Configuration {
    const int screenWidth;
    const int screenHeight;
public:
    std::array<int, 2> GetResolution() const;
    Configuration(int screenWidth, int screenHeight);
    ~Configuration();
};



#endif //CONFIGURATION_H
