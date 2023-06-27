#include "geometry.h"
#include "raylib.h"
#include "raymath.h"
#include <vector>

class Body
{
public:
  Vector3 position;
  Quaternion rotation;
  Shape *shape;
  Vector3 linearVelocity;
  float invertedMass; // allows use of (relatively) infinite masses - e.g. Earth
  // The ratio between an object’s velocity before and after a collision(0 - 1)
  // 1 = perfectly elastic, <1 = inelastic collision
  // source: https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/5collisionresponse/Physics%20-%20Collision%20Response.pdf
  float restitutionCoefficient;

  //   ~Body();

  enum Space
  {
    MODEL_SPACE,
    WORLD_SPACE
  };
  Vector3 GetCenterOfMassWorldSpace() const;
  Vector3 GetCenterOfMassModelSpace() const;

  Vector3 WorldSpaceToLocalSpace(const Vector3 &point) const;
  Vector3 LocalSpaceToWorldSpace(const Vector3 &point) const;

  void ApplyLinearImpulse(const Vector3 &impulse);
};

class Scene
{
public:
  void Initialize();
  //   void AddBody();
  //   void RemoveBody();
  void Update(const float dt_sec);

  std::vector<Body> bodies;
};

struct CollisionPoint
{
  Vector3 A_WorldSpace;
  Vector3 B_WorldSpace;

  Vector3 A_LocalSpace;
  Vector3 B_LocalSpace;

  Vector3 normal;       // world space
  float collisionDepth; // positive = no collision, negative = collision
  float impactTime;

  Body *bodyA;
  Body *bodyB;
};

bool Intersect(Body *bodyA, Body *bodyB, CollisionPoint &collisionPoint);
void ResolveContact(CollisionPoint &collisionPoint);