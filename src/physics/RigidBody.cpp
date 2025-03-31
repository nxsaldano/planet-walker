//
// Created by Victus on 22/3/2025.
//

#include "RigidBody.h"

#include "raymath.h"

RigidBody::RigidBody(): mass(1.0f), linearDamping(0), angularDamping(0), position(), orientation(),
                        velocity(), rotation(),
                        acceleration(),
                        transformMatrix(),
                        inverseInertiaTensor(),
                        inverseInertiaTensorWorld(),
                        forceAccum(),
                        isAwake(false), isGrounded(false),
                        lastFrameAcceleration(),
                        angularAcceleration(), gravityDirection() {
}

RigidBody::~RigidBody() = default;


static void CalculateTransformMatrix(Matrix &transformMatrix, const Vector3 &position,
                                     const Quaternion &orientation) {
    // Alias for readability
    const Quaternion &q = orientation;
    // Rotation matrix (3x3) components - column major order
    // First column (m0, m1, m2)
    transformMatrix.m0 = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
    transformMatrix.m1 = 2.0f*(q.x*q.y + q.w*q.z);
    transformMatrix.m2 = 2.0f*(q.x*q.z - q.w*q.y);
    // Second column (m4, m5, m6)
    transformMatrix.m4 = 2.0f*(q.x*q.y - q.w*q.z);
    transformMatrix.m5 = 1.0f - 2.0f*(q.x*q.x + q.z*q.z);
    transformMatrix.m6 = 2.0f*(q.y*q.z + q.w*q.x);
    // Third column (m8, m9, m10)
    transformMatrix.m8 = 2.0f*(q.x*q.z + q.w*q.y);
    transformMatrix.m9 = 2.0f*(q.y*q.z - q.w*q.x);
    transformMatrix.m10 = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
    // Translation column (m12, m13, m14)
    transformMatrix.m12 = position.x;
    transformMatrix.m13 = position.y;
    transformMatrix.m14 = position.z;
    // Set homogeneous coordinates (last row)
    transformMatrix.m3 = transformMatrix.m7 = transformMatrix.m11 = 0.0f;
    transformMatrix.m15 = 1.0f;
}

static void TransformInertiaTensor(Matrix &iitWorld, const Quaternion &q, const Matrix &iitBody,
    const Matrix &rotmat) {
    // Calculate rotation-transformed inertia tensor: I_world = R * I_body * R^T
    // Using matrix components directly for optimal performance
    // First column of rotation matrix (m0, m1, m2)
    const float& r0x = rotmat.m0;
    const float& r0y = rotmat.m1;
    const float& r0z = rotmat.m2;
    // Second column of rotation matrix (m4, m5, m6)
    const float& r1x = rotmat.m4;
    const float& r1y = rotmat.m5;
    const float& r1z = rotmat.m6;
    // Third column of rotation matrix (m8, m9, m10)
    const float& r2x = rotmat.m8;
    const float& r2y = rotmat.m9;
    const float& r2z = rotmat.m10;
    // Intermediate calculations for R * I_body
    const float t0x = r0x*iitBody.m0 + r1x*iitBody.m1 + r2x*iitBody.m2;
    const float t0y = r0x*iitBody.m4 + r1x*iitBody.m5 + r2x*iitBody.m6;
    const float t0z = r0x*iitBody.m8 + r1x*iitBody.m9 + r2x*iitBody.m10;
    const float t1x = r0y*iitBody.m0 + r1y*iitBody.m1 + r2y*iitBody.m2;
    const float t1y = r0y*iitBody.m4 + r1y*iitBody.m5 + r2y*iitBody.m6;
    const float t1z = r0y*iitBody.m8 + r1y*iitBody.m9 + r2y*iitBody.m10;
    const float t2x = r0z*iitBody.m0 + r1z*iitBody.m1 + r2z*iitBody.m2;
    const float t2y = r0z*iitBody.m4 + r1z*iitBody.m5 + r2z*iitBody.m6;
    const float t2z = r0z*iitBody.m8 + r1z*iitBody.m9 + r2z*iitBody.m10;
    // Compute final I_world = (R*I_body) * R^T
    // First column (m0, m1, m2)
    iitWorld.m0 = t0x*r0x + t0y*r1x + t0z*r2x;
    iitWorld.m1 = t0x*r0y + t0y*r1y + t0z*r2y;
    iitWorld.m2 = t0x*r0z + t0y*r1z + t0z*r2z;
    // Second column (m4, m5, m6)
    iitWorld.m4 = t1x*r0x + t1y*r1x + t1z*r2x;
    iitWorld.m5 = t1x*r0y + t1y*r1y + t1z*r2y;
    iitWorld.m6 = t1x*r0z + t1y*r1z + t1z*r2z;
    // Third column (m8, m9, m10)
    iitWorld.m8 = t2x*r0x + t2y*r1x + t2z*r2x;
    iitWorld.m9 = t2x*r0y + t2y*r1y + t2z*r2y;
    iitWorld.m10 = t2x*r0z + t2y*r1z + t2z*r2z;
    // Set remaining matrix elements to identity-like values
    iitWorld.m3 = iitWorld.m7 = iitWorld.m11 = 0.0f; // Zero bottom row
    iitWorld.m12 = iitWorld.m13 = iitWorld.m14 = 0.0f; // Zero translation
    iitWorld.m15 = 1.0f; // Homogeneous coordinate
}

void RigidBody::CalculateDerivedData() {
    orientation = QuaternionNormalize(orientation);
    CalculateTransformMatrix(transformMatrix, position, orientation);
    TransformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);
}

void RigidBody::SetInertiaTensor(const Matrix &inertiaTensor) {
    inverseInertiaTensor = MatrixInvert(inertiaTensor);
}

void RigidBody::AddForce(const Vector3 &force) {
    forceAccum += force;
    isAwake = true;
}

void RigidBody::ClearAccumulators() {
    forceAccum = Vector3Zero();
}

bool RigidBody::HasFiniteMass() const {
    return inverseMass != 0.0f;
}

float RigidBody::GetMass() const {
    return mass;
}

void RigidBody::SetMass(const float newMass) {
    mass = newMass;
    inverseMass = (mass == 0.0f) ? 0.0f : 1.0f / mass;
}

Vector3 RigidBody::GetForceAccum() const {
    return forceAccum;
}

void RigidBody::SetVelocity(const Vector3 newVelocity) {
    velocity = newVelocity;
}

Vector3 RigidBody::GetVelocity() const {
    return velocity;
}

void RigidBody::Integrate(const float duration) {
    // Calculate linear acceleration from force inputs.
    lastFrameAcceleration = acceleration;
    const Vector3 scaledForceAccum = Vector3Scale(forceAccum, inverseMass);
    lastFrameAcceleration += scaledForceAccum;
    // Calculate angular acceleration from torque inputs.
    // Vector3 angularAcceleration = inverseInertiaTensorWorld.transform(torqueAccum);
    // Adjust velocities.
    // Update linear velocity from both acceleration and impulse.
    const Vector3 scaledLastFrameAcceleration = Vector3Scale(lastFrameAcceleration, duration);
    velocity += scaledLastFrameAcceleration;
    // Update angular velocity from both acceleration and impulse.
    const Vector3 scaledAngularAcceleration = Vector3Scale(angularAcceleration, duration);
    rotation += scaledAngularAcceleration;
    // Impose drag.
    velocity *= pow(linearDamping, duration);
    rotation *= pow(angularDamping, duration);
    // Adjust positions.
    // Update linear position.
    const Vector3 scaledVelocity = Vector3Scale(velocity, duration);
    Vector3 newPosition = Vector3Add(position, scaledVelocity);
    SetPosition(newPosition);
    // Update angular position
    const float rotationAngle = Vector3Length(rotation) * duration;
    if (rotationAngle > 0.0f) {
        const Vector3 rotationAxis = Vector3Normalize(rotation);
        const Quaternion deltaQ = QuaternionFromAxisAngle(rotationAxis, rotationAngle);
        orientation = deltaQ * orientation;
    }
    CalculateDerivedData();
    ClearAccumulators();
}

Vector3 RigidBody::GetPosition() const {
    return position;
}

void RigidBody::SetPosition(const Vector3 newPosition) {
    position = newPosition;
    UpdateCenter();
}

float RigidBody::GetRotationAngle() const {
    return Vector3Angle(Vector3Zero(), rotation);
}

Vector3 RigidBody::GetCenter() const {
    return center;
}

float RigidBody::GetRadius() const {
    return radius;
}

void RigidBody::SetOrientation(const Quaternion newOrientation) {
    orientation = newOrientation;
}

Quaternion RigidBody::GetOrientation() const {
    return orientation;
}

void RigidBody::SetGrounded(bool grounded) {
    isGrounded = grounded;
}

bool RigidBody::IsGrounded() const {
    return isGrounded;
}

void RigidBody::SetGravityDirection(Vector3 newGravityDirection) {
    gravityDirection = newGravityDirection;
}

Vector3 RigidBody::CalculateMeshCenter(const Model &model) {
    const BoundingBox bounds = GetModelBoundingBox(model);
    // Calculate the true center of the mesh
    const Vector3 meshCenter = {
        (bounds.min.x + bounds.max.x) / 2.0f,
        (bounds.min.y + bounds.max.y) / 2.0f,
        (bounds.min.z + bounds.max.z) / 2.0f
    };
    return meshCenter;
}

void RigidBody::UpdateCenter() {
    center = Vector3Add(position, CalculateMeshCenter(model));
}


