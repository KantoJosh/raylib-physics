#include "physics.h"
#include <iostream>

Vector3 Body::GetCenterOfMassWorldSpace() const {
  const Vector3 com = shape->GetCenterOfMass();
  const Vector3 pos =
      Vector3Add(position, Vector3RotateByQuaternion(com, rotation));
  return pos;
}

Vector3 Body::GetCenterOfMassModelSpace() const {
  return shape->GetCenterOfMass();
}

Vector3 Body::WorldSpaceToLocalSpace(const Vector3 &point) const {
  Vector3 tmp = Vector3Subtract(point, GetCenterOfMassWorldSpace());
  Quaternion inverserotation = QuaternionInvert(rotation);
  Vector3 localSpace = Vector3RotateByQuaternion(tmp, inverserotation);
  return localSpace;
}

Vector3 Body::LocalSpaceToWorldSpace(const Vector3 &point) const {
  Vector3 worldSpace = Vector3Add(GetCenterOfMassWorldSpace(),
                                  Vector3RotateByQuaternion(point, rotation));
  return worldSpace;
}

void Body::ApplyLinearImpulse(const Vector3 &impulse) {
  if (invertedMass == 0.0f) {
    return;
  }

  // p = mv (momentum)
  // dp = m dv = J (impulse)
  // dv = J / m
  linearVelocity =
      Vector3Add(linearVelocity, Vector3Scale(impulse, invertedMass));
}

// Body::~Body() { delete shape; }

void Scene::Initialize() {
  Body body;
  body.position = Vector3{0, 100, 0};
  body.linearVelocity = Vector3{0, 0, 0};
  body.rotation = Quaternion{0, 0, 0, 1};
  body.invertedMass = 1.0f;
  body.shape = new Sphere(5.0f);
  bodies.push_back(body);

  body.position = Vector3{0, -1000, 0};
  body.linearVelocity = Vector3{0, 0, 0};
  body.rotation = Quaternion{0, 0, 0, 1};
  body.invertedMass = 0.0f;
  body.shape = new Sphere(1000.0f);
  bodies.push_back(body);
}

void Scene::Update(const float deltaTime) {
  // apply gravity as an impulse
  // impulse = dp = F * dt
  for (int i = 0; i < bodies.size(); i++) {
    Body *body = &bodies[i];
    float mass = 1 / body->invertedMass;
    Vector3 impulseGravity =
        Vector3Scale(Vector3{0, -9.8f, 0}, mass * deltaTime);
    body->ApplyLinearImpulse(impulseGravity);
    // std::cout << "velocity " << body.linearVelocity.x << " "
    //           << body.linearVelocity.y << " " << body.linearVelocity.z
    //           << std::endl;
  }

  // detect collisions
  for (int i = 0; i < bodies.size(); i++) {
    for (int j = i + 1; j < bodies.size(); j++) {
      Body *bodyA = &bodies[i];
      Body *bodyB = &bodies[j];

      if (bodyA->invertedMass == 0.0f && bodyB->invertedMass == 0.0f) {
        continue;
      }

      CollisionPoint collisionPoint;
      if (Intersect(bodyA, bodyB, collisionPoint)) {
        ResolveContact(collisionPoint);
      }
    }
  }

  // apply position change due to velocity
  for (auto &body : bodies) {
    Vector3 deltaPosition = Vector3Scale(body.linearVelocity, deltaTime);
    // std::cout << "velocity: " << body.linearVelocity.x << " "
    //           << body.linearVelocity.y << " " << body.linearVelocity.z
    //           << std::endl;
    // std::cout << "delta pos: " << deltaPosition.x << " " << deltaPosition.y
    //           << " " << deltaPosition.z << " deltatime" << deltaTime
    //           << std::endl;
    body.position = Vector3Add(body.position, deltaPosition);
  }
}

bool Intersect(Body *bodyA, Body *bodyB, CollisionPoint &collisionPoint) {
  collisionPoint.bodyA = bodyA;
  collisionPoint.bodyB = bodyB;
  // sphere - sphere intersection
  if (bodyA->shape->GetType() == Shape::SPHERE &&
      bodyB->shape->GetType() == Shape::SPHERE) {
    Vector3 AtoB = Vector3Subtract(bodyB->position, bodyA->position);
    collisionPoint.normal = Vector3Normalize(AtoB);

    Sphere *sphereA = dynamic_cast<Sphere *>(bodyA->shape);
    Sphere *sphereB = dynamic_cast<Sphere *>(bodyB->shape);

    collisionPoint.A_WorldSpace = Vector3Add(
        bodyA->position, Vector3Scale(collisionPoint.normal, sphereA->radius));
    collisionPoint.B_WorldSpace =
        Vector3Add(bodyB->position,
                   Vector3Scale(collisionPoint.normal, -1 * sphereB->radius));
    // distance between their centers is < both
    // radii added up == intersection
    float distanceBetweenSqr = Vector3LengthSqr(AtoB);
    float radiusSum = sphereA->radius + sphereB->radius;
    return distanceBetweenSqr <= (radiusSum * radiusSum);
  }
  return false;
}

void ResolveContact(CollisionPoint &collisionPoint) {
  // zero out body velocities
  collisionPoint.bodyA->linearVelocity = Vector3Zero();
  collisionPoint.bodyB->linearVelocity = Vector3Zero();

  Vector3 distance =
      Vector3Subtract(collisionPoint.B_WorldSpace, collisionPoint.A_WorldSpace);
  Vector3 A_WorldPositionDelta =
      Vector3Scale(distance, (collisionPoint.bodyA->invertedMass) /
                                 (collisionPoint.bodyA->invertedMass +
                                  collisionPoint.bodyB->invertedMass));
  Vector3 B_WorldPositionDelta =
      Vector3Scale(distance, (collisionPoint.bodyB->invertedMass) /
                                 (collisionPoint.bodyA->invertedMass +
                                  collisionPoint.bodyB->invertedMass));
  collisionPoint.bodyA->position =
      Vector3Add(collisionPoint.bodyA->position, A_WorldPositionDelta);
  collisionPoint.bodyB->position =
      Vector3Add(collisionPoint.bodyB->position, B_WorldPositionDelta);

  // shift colliders so they are not inter
}