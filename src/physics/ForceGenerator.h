//
// Created by Victus on 23/3/2025.
//

#ifndef FORCEGENERATOR_H
#define FORCEGENERATOR_H
#include "RigidBody.h"


class ForceGenerator {
public:
    virtual void UpdateForce(RigidBody *body, float duration) = 0;
};



#endif //FORCEGENERATOR_H
