//
// Created by Victus on 23/3/2025.
//

#ifndef GRAVITY_H
#define GRAVITY_H
#include "ForceGenerator.h"


class Gravity : public ForceGenerator {
    Vector3 gravity;
public:
    explicit Gravity(const Vector3& gravity);
    void UpdateForce(RigidBody *body, float duration) override;
    Vector3 GetGravity();
};



#endif //GRAVITY_H
