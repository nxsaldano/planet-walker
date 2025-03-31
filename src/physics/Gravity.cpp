//
// Created by Victus on 23/3/2025.
//

#include "Gravity.h"

#include "raymath.h"

Gravity::Gravity(const Vector3 &gravity) : gravity(gravity) {}

void Gravity::UpdateForce(RigidBody *body, float duration) {
    // Check that we do not have infinite mass.
    if (!body->HasFiniteMass()) {
        TraceLog(LOG_INFO, "Infinite mass detected.");
        return;
    }
    body->AddForce(Vector3Scale(gravity, body->GetMass()));
}

Vector3 Gravity::GetGravity() {
    return gravity;
}
