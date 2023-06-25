#include "raylib.h"
#include "raymath.h"

class Shape {
public:
  enum ShapeType {
    SPHERE,
  };

  virtual ShapeType GetType() const = 0;
  virtual Vector3 GetCenterOfMass() const { return centerOfMass; }

protected:
  Vector3 centerOfMass;
};

class Sphere : public Shape {
public:
  Sphere(float radius) : radius{radius} {}
  ShapeType GetType() const override { return SPHERE; }
  float radius;
};
