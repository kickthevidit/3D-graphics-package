#pragma once

#include "shape.h"

class Sphere : public Shape {
  Vec3 center = Vec3(0, 0, 0);
  double radius = 1;

public:
  Sphere() = default;
  Sphere(const Vec3 &c, double r) : Shape(), center(c), radius(r) {
    if (radius < 0)
      radius *= -1;
  }
  Sphere(const Vec3 &c, double r, Pixel &p, double d, Vec3 al)
      : Shape(p, d, al), center(c), radius(r) {
    if (radius < 0)
      radius *= -1;
  }
  Sphere(const Vec3 &c, double r, Pixel &p) : Shape(p), center(c), radius(r) {
    if (radius < 0)
      radius *= -1;
  }
  Sphere(const Vec3 &c, double r, Material &m)
      : Shape(m), center(c), radius(r) {
    if (radius < 0)
      radius *= -1;
  }

  Vec3 GetCenter() const { return center; }
  virtual Vec3 GetNormal(const Vec3 &point) const;

  virtual bool RayIntersect(const Vec3 &orig, const Vec3 &dir,
                            double &t0) const;
};
