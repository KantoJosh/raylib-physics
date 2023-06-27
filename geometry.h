#include "raylib.h"
#include "raymath.h"

class Shape
{
public:
  enum ShapeType
  {
    SPHERE,
  };

  virtual ShapeType GetType() const = 0;
  virtual Vector3 GetCenterOfMass() const { return centerOfMass; }
  virtual Matrix GetInertiaTensor() const = 0;

protected:
  Vector3 centerOfMass;
};

class Sphere : public Shape
{
public:
  Sphere(float radius) : radius{radius}
  {
    centerOfMass = Vector3Zero();
  }

  ShapeType GetType() const override { return SPHERE; }

  Matrix GetInertiaTensor() const
  {
    Matrix m;
    m.m0 =
  }
  float radius;
};
