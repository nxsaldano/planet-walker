//
// Created by Victus on 22/3/2025.
//

#ifndef RIGIDBODY_H
#define RIGIDBODY_H
#include "raylib.h"
#include "raymath.h"


class RigidBody {
protected:
    Model model{};
    bool isAwake;
    bool isGrounded;
    float mass;
    float inverseMass{};
    float linearDamping;
    float angularDamping;
    float radius{};
    Quaternion orientation = QuaternionIdentity();
    Vector3 position;
    Vector3 center{};
    Vector3 velocity;
    Vector3 rotation;
    Vector3 acceleration;
    Vector3 forceAccum;
    Vector3 lastFrameAcceleration;
    Vector3 angularAcceleration;
    Vector3 gravityDirection;
    Matrix transformMatrix;
    Matrix inverseInertiaTensor;
    Matrix inverseInertiaTensorWorld;
    void CalculateDerivedData();
    void SetInertiaTensor(const Matrix &inertiaTensor);
    void AddForceAtPoint(const Vector3 &force, const Vector3 &point);
    void AdddForceAtBodyPoint(const Vector3 &force, const Vector3 &point);
    void ClearAccumulators();
    static Vector3 CalculateMeshCenter(const Model &model);
    void UpdateCenter();
public:
    RigidBody();
    ~RigidBody();
    void Integrate(float duration);
    void AddForce(const Vector3 &force);
    bool HasFiniteMass() const;
    float GetMass() const;
    void SetMass(float newMass);
    Vector3 GetForceAccum() const;
    void SetVelocity(Vector3 newVelocity);
    Vector3 GetVelocity() const;
    Vector3 GetPosition() const;
    void SetPosition(Vector3 newPosition);
    float GetRotationAngle() const;
    Vector3 GetCenter() const;
    float GetRadius() const;
    void SetOrientation(Quaternion newOrientation);
    Quaternion GetOrientation() const;
    void SetGrounded(bool grounded);
    bool IsGrounded() const;
    void SetGravityDirection(Vector3 newGravityDirection);
};



#endif //RIGIDBODY_H
